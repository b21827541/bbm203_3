#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

typedef struct gene{
    int val;
    struct gene* next;
}gene;
typedef struct list{
    gene* head_gene;
    struct list* next;
    int fitness;
    int rank;

}chromosome;
char* file_to_string(char* path){
    FILE* file = fopen(path, "r");
    int length = 0;
    char* string ;

    if (!file){
        printf("%s", "asd");
        return NULL;
    }
    int c = fgetc(file);

    while (c != EOF){
        length++;
        string = realloc(string, (length + 1) * sizeof(char));
        string[length-1] = (char) c;
        c = fgetc(file);
    }
    string[length] = '\0';

    fclose(file);

    return string;
}

void PrintChromosomes(chromosome* head_chromosome){
    chromosome* current_chromosome = head_chromosome;
    while(current_chromosome){
        gene* current_gene = current_chromosome->head_gene;
        while(current_gene){
            printf("%d ",current_gene->val);
            current_gene = current_gene->next;
        }
        printf("\n");
        current_chromosome = current_chromosome->next;
    }

}

int pow1(int exponent){
    int i;
    int power = 1;
    for(i=1;i<=exponent;i++)
    {
        power*=2;
    }
    return power;
}

int FitnessCalculation(chromosome* curr_chromosome, int prob_size){
    chromosome* curr = curr_chromosome;
    int x = 0;
    int y = prob_size-1;
    gene* curr_gene = curr->head_gene;
    while(curr_gene){
        x = x + curr_gene->val * pow1(y);
        y--;
        curr_gene = curr_gene->next;
    }
    return x;
}

int FitnessBest(chromosome* head_chromosome){
    int best;
    int flag = 1;
    chromosome* current_chromosome = head_chromosome;
    while(current_chromosome){
        gene* current_gene = current_chromosome->head_gene;
        while(current_gene){
            if(flag){
                best = current_gene->val;
                current_gene = current_gene->next;
                flag = 0;
                continue;
            }
            if(best < current_gene->val){
                current_gene = current_gene->next;
                continue;
            }
            best = current_gene->val;
            current_gene = current_gene->next;
        }
        current_chromosome = current_chromosome->next;
    }
    return best;
}

int FitnessWorst(chromosome* head_chromosome){
    int worst;
    int flag = 1;
    chromosome* current_chromosome = head_chromosome;
    while(current_chromosome){
        gene* current_gene = current_chromosome->head_gene;
        while(current_gene){
            if(flag){
                worst = current_gene->val;
                current_gene = current_gene->next;
                flag = 0;
                continue;
            }
            if(worst > current_gene->val){
                current_gene = current_gene->next;
                continue;
            }
            worst = current_gene->val;
            current_gene = current_gene->next;
        }
        current_chromosome = current_chromosome->next;
    }
    return worst;
}

void RankCalculation(chromosome* head_chromosome, int best_fit, int worst_fit){
    chromosome* current_chromosome = head_chromosome;
    while(current_chromosome){
        if(best_fit == worst_fit) {
            current_chromosome->rank = NULL;
        }else {
            current_chromosome->rank = (current_chromosome->fitness - best_fit) / (worst_fit - best_fit);
            current_chromosome = current_chromosome->next;
        }
    }
}

chromosome* NewChromosome(){
    chromosome* new_chromosome =  (chromosome*)malloc(sizeof(chromosome));
    new_chromosome->next = NULL;
    new_chromosome->head_gene = NULL;
    new_chromosome->fitness = -5;
    new_chromosome->rank = -5;
}

gene* NewGene(int val){
    gene* new_gene = (gene*)malloc(sizeof(gene));
    new_gene->val = val;
    new_gene->next = NULL;
}

void InsertChromosome(chromosome* head_chromosome, chromosome* inserted){
    chromosome* current = head_chromosome;
    while(current->next != NULL){
        current = current->next;
    }
    current->next = inserted;
}

chromosome* FindChromosome(chromosome* head_chromosome, int number){
    int counter = 1;
    chromosome* current = head_chromosome;
    while(current){
        if(counter == number)
            return current;
        counter++;
        current = current->next;
    }
    return NULL;

}

void InsertGene(chromosome* head_chromosome, int val){
    gene** head_gene = &(head_chromosome->head_gene);
    if(*head_gene == NULL){
        *head_gene = NewGene(val);
    }
    else{
        gene* current = *head_gene;
        while(current->next){
            current = current->next;
        }
        current->next = NewGene(val);
    }
}



int main(int argc, char* argv[]) {
    int i;
    char* input_lines = file_to_string("population");
    char* rest = input_lines;
    char* line;
    FILE* output = fopen("output", "w+");
    char *order;
    char* rest2;
    char *ptr;
    long num;
    int best_fitness, worst_fitness;
    int pop_size = atoi(*(argv+2));// *(argv+2) olacak
    int prob_size = atoi(*(argv+1));
    //int MAX_GEN = atoi(argv[3]);
    int name = 1;

    // Creating chromosomes according to pop_size
    chromosome* head_chromosome = NewChromosome();
    for(i = 1; i < pop_size; i++){
        chromosome* current = head_chromosome;
        while(current->next)
            current = current->next;
        current->next = NewChromosome();
    }

    while ((line = strtok_r(rest, "\r\n", &rest))){
        rest2 = line;
        chromosome* current_chromosome = FindChromosome(head_chromosome, name);
        while((order = strtok_r(rest2, ":", &rest2))) {
            if(strcmp(order, "\r\n") == 0) {
                *(order + strlen(order) - 2) = '\0';
                num = strtol(order, &ptr, 10);
            }else {
                num = strtol(order, &ptr, 10);
            }
            InsertGene(current_chromosome,num);
        }
        current_chromosome->fitness = FitnessCalculation(current_chromosome, prob_size);
        name++;
    }
    best_fitness = FitnessBest(head_chromosome);
    worst_fitness = FitnessWorst(head_chromosome);
    RankCalculation(head_chromosome, best_fitness, worst_fitness);
    PrintChromosomes(head_chromosome);
    // Sort chromosomes according to fitness ascendingly

    return 0;
}