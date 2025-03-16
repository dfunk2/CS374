#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MEM_SIZE 16384  // MUST equal PAGE_SIZE * PAGE_COUNT
#define PAGE_SIZE 256  // MUST equal 2^PAGE_SHIFT
#define PAGE_COUNT 64
#define PAGE_SHIFT 8  // Shift page number this much

#define PTP_OFFSET 64 // How far offset in page 0 is the page table pointer table

//global counter 
int functionCalled = 0; 

// Simulated RAM
unsigned char mem[MEM_SIZE];

//
// Convert a page,offset into an address
//
int get_address(int page, int offset)
{
    return (page << PAGE_SHIFT) | offset; 
}

//
// Initialize RAM
//
void initialize_mem(void)
{
    memset(mem, 0, MEM_SIZE);

    int zpfree_addr = get_address(0, 0);
    mem[zpfree_addr] = 1;  // Mark zero page as allocated
}

//
// Get the page table page for a given process
//
unsigned char get_page_table(int proc_num)
{
    int ptp_addr = get_address(0, PTP_OFFSET + proc_num);
    return mem[ptp_addr];
}

//
// Allocate pages for a new process
//
// This includes the new process page table and page_count data pages.
//
void new_process(int proc_num, int page_count)
{  
    int page_table = -1; 

    //iterate through free page map for free slots and allocate slot for page table
    for(int j = 0; j < PAGE_COUNT; j++){
        int free_map_addr = get_address(0, j);
        if(mem[free_map_addr] == 0){
            page_table = j;
            mem[free_map_addr] = 1;  
            break;
        }
    }
    if(page_table == -1){
        printf("OOM: proc %d: page table\n", proc_num);
        return;
    }
        //get virtual address in page 0 (in page table map) of page table pointer
        int ptp_addr = get_address(0, PTP_OFFSET + proc_num); 
        //assign page table to a physical address
        mem[ptp_addr] = page_table; 
        
        //iterate through number of pages, process requested and assign them to a slot in the page table
        int page_entry = -1;
        for(int i = 0; i < page_count; i++){ 

            //search for free pages again
            for(int k = 0; k < PAGE_COUNT; k++){
                int addr = get_address(0, k); 
                if(mem[addr] == 0){
                    page_entry = k;
                    mem[addr] = 1;
                    break;
                }   
            } 
            if(page_entry == -1){
                printf("OOM: proc %d: data page\n", proc_num);
                return;
            }

                //get virtual address for page table entry inside page table
                int addr_page_entry = get_address(page_table, i);
                //assign page entry iteration into page table's physical address 
                mem[addr_page_entry] = page_entry; 
        } 
}

//
//kill a process
//
void kill_process(int proc_num){
    
    //get virtual address in page 0 (in page table map) of page table pointer
    int ptp_addr = get_address(0, PTP_OFFSET + proc_num); 
    //assign page table to a physical address
    int page_table = mem[ptp_addr]; 
       
    //iterate through number of pages in page table and remove them
    
       for(int i = 0; i < PAGE_COUNT; i++){ 
            //get virt addr of page at index i in page table
            int page_entry_addr = get_address(page_table, i);
            
            //map to its physical addr
            int phys_page_entry_addr = mem[page_entry_addr];

            //get virt addr of that page in page 0 in map of used pages
            int used_pages_addr = get_address(0, phys_page_entry_addr);

            //check if physical address is anything other than 0
            if(phys_page_entry_addr != 0){
               
                //mark physical page of page entry in zero page as 0/unused
                mem[used_pages_addr] = 0;
                
                } else {
                    continue;
            }
        }
        //get virt addr of page table in zero page
        int used_pt_addr = get_address(0, page_table);
        //map to its physical page and then set to unused
        mem[used_pt_addr] = 0;     
}


int store_val(int proc_num, int vaddr, int val){
    //get page table
    int ptp_addr = get_address(0, PTP_OFFSET + proc_num);
    int page_table = mem[ptp_addr];

    //what page are we talk about? given an address, pull page number from it

        int virtual_page = vaddr >> 8;

        int offset = vaddr & 255;
        
        int page_entry_addr = get_address(page_table, virtual_page);
        
    //build physical address 
    
        int phys_page = mem[page_entry_addr];

        int phys_addr = (phys_page << 8) | offset;
    //store value
        mem[vaddr] = val;

    if(functionCalled == 0){
        printf("Store proc %d: %d => %d, value=%d\n",
            proc_num, vaddr, phys_addr, val);
        functionCalled++;
    }
    return phys_addr;
}

void load_val(int proc_num, int vaddr){
    //load value
        int val = mem[vaddr];
        int phys_addr = store_val(proc_num, vaddr, val);
    
        printf("Load proc %d: %d => %d, value=%d\n",
                proc_num, vaddr, phys_addr, val);
}

//
// Print the free page map
//
// Don't modify this
//
void print_page_free_map(void)
{
    printf("--- PAGE FREE MAP ---\n");

    for (int i = 0; i < 64; i++) {
        int addr = get_address(0, i);

        printf("%c", mem[addr] == 0? '.': '#');

        if ((i + 1) % 16 == 0)
            putchar('\n');
    }
}

//
// Print the address map from virtual pages to physical
//
// Don't modify this
//
void print_page_table(int proc_num)
{
    printf("--- PROCESS %d PAGE TABLE ---\n", proc_num);

    // Get the page table for this process
    int page_table = get_page_table(proc_num);

    // Loop through, printing out used pointers
    for (int i = 0; i < PAGE_COUNT; i++) {
        int addr = get_address(page_table, i);

        int page = mem[addr];

        if (page != 0) {
            printf("%02x -> %02x\n", i, page);
        }
    }
}

//
// Main -- process command line
//
int main(int argc, char *argv[])
{
    assert(PAGE_COUNT * PAGE_SIZE == MEM_SIZE);

    if (argc == 1) {
        fprintf(stderr, "usage: ptsim commands\n");
        return 1;
    }
    
    initialize_mem();

    for (int i = 1; i < argc; i++) {
        //pfm - print a representation of a free map

        if (strcmp(argv[i], "pfm") == 0) { 
            print_page_free_map();
        }
        //ppt n - print page table for process n

        else if (strcmp(argv[i], "ppt") == 0) {
            int proc_num = atoi(argv[++i]);
            print_page_table(proc_num);
        } 
        //np n m - launch new process n with initial allocation of m pages
        else if (strcmp(argv[i], "np") == 0){
            int proc_num = atoi(argv[i+1]);
            int page_count = atoi(argv[i+2]);
            new_process(proc_num, page_count);
        }

        //kp n - kill process n and free all it's pages
        else if (strcmp(argv[i], "kp") == 0){
            int proc_num = atoi(argv[i+1]);
            kill_process(proc_num);
        }

        //sb n a b - for process n at virtual address a, store the value b
        else if (strcmp(argv[i], "sb") == 0) {
            int proc_num = atoi(argv[i+1]);
            int vaddr = atoi(argv[i+2]);
            int val = atoi(argv[i+3]);
            store_val(proc_num, vaddr, val);
        }

        //lb n a - for process n, get the value at virtual address a
        else if(strcmp(argv[i], "lb") ==0) {
            int proc_num = atoi(argv[i+1]);
            int vaddr = atoi(argv[i+2]);
            load_val(proc_num, vaddr);
        }
    }
}
