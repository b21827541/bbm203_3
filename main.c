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
    double rank;

}chromosome;
char* file_to_string(char* path){
    FILE* file = fopen(path, "r");
    int length = 0;
    char* string = malloc(0);

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
            if(current_gene->next == NULL){
                printf("%d ", current_gene->val);
                break;
            }
            printf("%d:",current_gene->val);
            current_gene = current_gene->next;
        }
        printf(" -> %d", current_chromosome->fitness);
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
        /*gene* current_gene = current_chromosome->head_gene;
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
        }*/
        if(flag){
            best = current_chromosome->fitness;
            current_chromosome = current_chromosome->next;
            flag = 0;
            continue;
        }
        if(best < current_chromosome->fitness){
            current_chromosome = current_chromosome->next;
            continue;
        }
        best = current_chromosome->fitness;
        current_chromosome = current_chromosome->next;
    }
    return best;
}

int FitnessWorst(chromosome* head_chromosome){
    int worst;
    int flag = 1;
    chromosome* current_chromosome = head_chromosome;
    while(current_chromosome){
        /*gene* current_gene = current_chromosome->head_gene;
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
        }*/
        if(flag){
            worst = current_chromosome->fitness;
            current_chromosome = current_chromosome->next;
            flag = 0;
            continue;
        }
        if(worst > current_chromosome->fitness){
            current_chromosome = current_chromosome->next;
            continue;
        }
        worst = current_chromosome->fitness;
        current_chromosome = current_chromosome->next;
        //current_chromosome = current_chromosome->next;
    }
    return worst;
}

void RankCalculation(chromosome* head_chromosome, double best_fit, double worst_fit){
    chromosome* current_chromosome = head_chromosome;
    while(current_chromosome){
        if(best_fit == worst_fit) {
            continue;//                   NULL OLMASINI ISTEMISLER
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


void FrontBackSplit(chromosome* source, chromosome** first, chromosome** last)
{
    chromosome* two_nodes;
    chromosome* one_node;
    one_node = source;
    two_nodes = source->next;

    while (two_nodes != NULL) {
        two_nodes = two_nodes->next;
        if (two_nodes != NULL) {
            one_node = one_node->next;
            two_nodes = two_nodes->next;
        }
    }
    // split into two
    *first = source;
    *last = one_node->next;
    one_node->next = NULL;
}

chromosome* Merge(chromosome* first, chromosome* second)
{
    chromosome* result = NULL;

    if (first == NULL) {
        return (second);
    }else if (second == NULL) {
        return (first);
    }
    //pick one of them according to fitness
    if (first->fitness <= second->fitness) {
        result = first;
        result->next = Merge(first->next, second);
    }
    else {
        result = second;
        result->next = Merge(first, second->next);
    }

    return (result);
}

void Sort(chromosome** head_chromosome){
    chromosome* head = *head_chromosome;
    chromosome* first;
    chromosome* second;

    if ((head == NULL) || (head->next == NULL)) {
        return;
    }
    //divide head to two lists
    FrontBackSplit(head, &first, &second);
    //sort two lists
    Sort(&first);
    Sort(&second);
    // merge sorted two lists
    *head_chromosome = Merge(first, second);
}

void Evolve(int a, int b, int x, int y, chromosome* head_chromosome){
    int i=1,k=1,j=1, temp;    //j = 1 olabilir
    chromosome* first = head_chromosome;
    chromosome* second = head_chromosome;
    gene* first_gene;
    gene* second_gene;
    while(i<a){
        first = first->next;
        i++;
    }
    //if(first->next == second->next)
    while(k<b){
        second = second->next;
        k++;
    }
    first_gene = first->head_gene;
    second_gene = second->head_gene;
    while(j<x){
        first_gene=first_gene->next;
        second_gene=second_gene->next;
        j++;
    }
    while(j<=y){
        temp = first_gene->val;
        first_gene->val = second_gene->val;
        second_gene->val = temp;
        first_gene = first_gene->next;
        second_gene = second_gene->next;
        j++;
    }

}
void Mutate(int a, int b, int z, chromosome* head_chromosome, int prob_size){
    int i=1,k=1,j=1;
    chromosome* first = head_chromosome;
    chromosome* second = head_chromosome;
    gene* first_gene;
    gene* second_gene;
    while(i<a){
        first = first->next;
        i++;
    }
    while(k<b){
        second = second->next;
        k++;
    }

    first_gene = first->head_gene;
    second_gene = second->head_gene;

    while(j<z){
        first_gene = first_gene->next;
        second_gene = second_gene->next;
        j++;
    }

    if(first_gene->val == 1)
        first_gene->val = 0;
    else
        first_gene->val = 1;
    if(second_gene->val == 1)
        second_gene->val = 0;
    else
        second_gene->val =1;
    first->fitness = FitnessCalculation(first,prob_size);
    second->fitness = FitnessCalculation(second,prob_size);
}

void PrintBestChromosome(chromosome* best_chromosome){
    chromosome* curr = best_chromosome;
    gene* curr_gene = curr->head_gene;
    printf("Best chromosome found so far: ");
    while(curr_gene){
        if(curr_gene->next == NULL) {
            printf("%d", curr_gene->val);
            break;
        }
        printf("%d:", curr_gene->val);
        curr_gene = curr_gene->next;
    }
    printf(" -> %d", curr->fitness);
}

chromosome* BestChromosome(int generation,chromosome* head, chromosome* best){
    chromosome* curr = head;
    gene* curr_gene = curr->head_gene;
    if(!generation){
        while(curr_gene){
            InsertGene(best,curr_gene->val);
            curr_gene=curr_gene->next;
        }
        best->fitness = curr->fitness;
        best->rank = curr->rank;
        return best;
    }
    if(curr->fitness < best->fitness){
        gene* new_gene = best->head_gene;
        while(curr_gene){
            new_gene->val = curr_gene->val;
            new_gene=new_gene->next;
            curr_gene=curr_gene->next;
        }
        best->fitness = curr->fitness;
        best->rank = curr->rank;
    }
    return best;
}

int main(int argc, char* argv[]) {
    int i, a, b, x, y, z, generation = 0;
    int name = 1;
    char* input_lines = file_to_string("population"),* selection = file_to_string("selection"),* xover = file_to_string("xover"),* mutate = file_to_string("mutate");
    char* rest = input_lines,* selection_rest = selection,* xover_rest = xover,* mutate_rest = mutate;
    char* rest2;
    char* line;
    char *order;
    char *ptr;
    long num;
    int best_fitness, worst_fitness;
    int pop_size = atoi(*(argv+2)),prob_size = atoi(*(argv+1)),MAX_GEN = atoi(*(argv+3));
    char* line_selection, *line_xover, *selection_rest2, *order2, *line_mutate;

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

    RankCalculation(head_chromosome, (double) best_fitness, (double) worst_fitness);

    // Sort chromosomes according to fitness ascending
    Sort(&head_chromosome);

    chromosome* best_chromosome = NewChromosome();
    //Evolving and Mutating the chromosomes for every generation
    while ((line_selection = strtok_r(selection_rest, "\r\n", &selection_rest))) {
        if(generation == 0){
            printf("\n");
            printf("Generation: %d \n", generation);
            PrintChromosomes(head_chromosome);

            best_chromosome = BestChromosome(generation,head_chromosome, best_chromosome);
            PrintBestChromosome(best_chromosome);
            generation++;
        }
        selection_rest2 = line_selection;
        line_xover = strtok_r(xover_rest, "\r\n", &xover_rest);
        x = atoi(strtok_r(line_xover, ":", &line_xover));
        y = atoi(strtok_r(line_xover, "\r\n", &line_xover));
        line_mutate = strtok_r(mutate_rest, "\r\n", &mutate_rest);
        z = atoi(line_mutate);
        while((order2 = strtok_r(selection_rest2, " ", &selection_rest2))) {
             a = atoi(strtok_r(order2,":",&order2));
             b = atoi(strtok_r(order2,":",&order2));
             Evolve(a,b,x,y,head_chromosome);
             Mutate(a,b,z,head_chromosome,prob_size);
        }
        printf("\n");
        printf("Generation: %d \n", generation);
        best_fitness = FitnessBest(head_chromosome);
        worst_fitness = FitnessWorst(head_chromosome);
        RankCalculation(head_chromosome, (double) best_fitness, (double) worst_fitness);
        Sort(&head_chromosome);
        PrintChromosomes(head_chromosome);

        best_chromosome = BestChromosome(generation,head_chromosome, best_chromosome);
        PrintBestChromosome(best_chromosome);
        generation++;
    }


    printf("\n");

    //free
    chromosome* tmp ;
    while(head_chromosome){
        tmp = head_chromosome;
        gene* gene1 = head_chromosome->head_gene;
        gene* temp ;
        while(gene1 != NULL){
            temp = gene1;
            gene1 = gene1->next;
            free(temp);
        }
        head_chromosome = head_chromosome->next;
        free(tmp);
        //free(head_chromosome);
    }
    free(input_lines);
    free(selection);
    free(xover);
    free(mutate);
    gene* gene2 = best_chromosome->head_gene;
    gene* temp2;
    while(gene2 != NULL){
        temp2 = gene2;
        gene2 = gene2->next;
        free(temp2);
    }
    free(best_chromosome);
    return 0;
}