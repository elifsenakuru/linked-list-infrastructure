#include <stdio.h>
#include <stdlib.h>
#include "the3.h"

/*last version*/

Apartment* add_apartment(Apartment* head, int index, char* apartment_name, int max_bandwidth){
	
	Apartment* temp1 = head, *tail = head;
	Apartment* temp2= head;

	Apartment* new_apartment = malloc(sizeof(Apartment));
	new_apartment -> name = apartment_name;
	new_apartment -> max_bandwidth = max_bandwidth;

	
	/* no apartment in street*/
	if (head == NULL){
		new_apartment -> next = new_apartment;
		return new_apartment;
	}
	
	else{
		while(tail -> next!= head){
			tail = tail->next; /*last apartment*/
		}
		
		if (index == 0){
			new_apartment -> next = head;
			tail -> next = new_apartment;
			return new_apartment;
		}
		
		else{
			while (temp2 -> next != head){
				temp2 = temp2 -> next;
			}
			
			while (index > 1){
				temp1 = temp1 -> next;
				index--;
			}
			
			new_apartment -> next = temp1 -> next;
			temp1 -> next = new_apartment;
			
			if (temp1 == temp2){
				temp2 = temp2 ->next;
			}
			return head;		
		}	
	}	
}

int helper1(char* a, char* b){
	int i=0;
	
	for(i=0; *(a+i)==*(b+i); i++){
		if (*(a+i)== '\0' && *(b+i) == '\0') return 1;
	}
	return 0;
}

void add_flat(Apartment* head, char* apartment_name, int index, int flat_id, int initial_bandwidth){
	
	int total_bandwidth=0,new_band;
	Apartment* temp = head;
	Flat* true_aprt=NULL, *p=NULL;
	Flat* new_flat = malloc(sizeof(Flat));
	new_flat -> id = flat_id; new_flat -> is_empty =0; 
	new_flat ->prev = NULL; new_flat -> next = NULL;
	
	while(temp -> next != head){
		if (helper1(temp -> name, apartment_name)){
			break;
	    }
	    temp = temp ->next; /*find the wanted apartment*/  
	}
	true_aprt = temp -> flat_list;
	
	while (true_aprt != NULL){
		total_bandwidth += true_aprt -> initial_bandwidth;
		true_aprt = true_aprt ->next; /* flatleri dolas*/
	}
	true_aprt = temp -> flat_list; /*geri ilk hale donmeli*/
	
	if (true_aprt == NULL){
		temp ->flat_list = new_flat; /*if there is no apartment new_flat equals to flat_list*/
	}
	
	else if (index == 0){
		temp -> flat_list =new_flat;
		new_flat -> next = true_aprt;
		true_aprt -> prev =new_flat;
	}
	
	else{
		p = temp -> flat_list;
		while (index >1){
			p = p->next; /* find prev of needed flat*/
			index--;
		}

		if (p->next ==NULL){
			new_flat ->next =NULL;
			p ->next =new_flat;
			new_flat -> prev = p;
		}
		
		new_flat -> next = p ->next; /*intemediate nodes*/
		p ->next ->prev = new_flat;
		p ->next = new_flat;
		new_flat -> prev = p;

		new_band = temp -> max_bandwidth - total_bandwidth;
	}
	
	if (new_band >= initial_bandwidth){
		new_flat -> initial_bandwidth = initial_bandwidth;
	}
	else{
		new_flat->initial_bandwidth = initial_bandwidth = new_band;
	}
}

void free_flats(Apartment* to_be_removed){
	Flat* temp= to_be_removed ->flat_list, *next_temp;
	
	if(temp !=NULL){
		while(temp){
			next_temp = temp->next;
			free(temp);
			temp = next_temp;
		}
	} 
}

Apartment* remove_apartment(Apartment* head, char* apartment_name){
	Apartment* tp =head, *tp2 =NULL,*tail =head;
	
	if(head ->next == head){
		free_flats(head);
		free(head);
		return NULL;
	}

	if (helper1(head ->name, apartment_name))
	{
		while(tail -> next!= head) {
			tail = tail->next;
		}
		tp2 = tail;
		head = head ->next;
	}
	
	while (!(helper1(tp ->name, apartment_name))){
		tp2 = tp;
		tp = tp ->next;
	}
	tp2 ->next = tp ->next;
	
	free_flats(tp);
	free(tp);
	return head;
}


void make_flat_empty(Apartment* head, char* apartment_name, int flat_id){
	
	Apartment* temp = head;
	Flat* rem_flat= NULL;
	while (temp -> next != head){
		if (helper1(temp -> name,apartment_name)){
			rem_flat = temp ->flat_list;
			while (rem_flat){
				if (rem_flat -> id == flat_id){
					rem_flat -> is_empty =1;
					rem_flat -> initial_bandwidth = 0;
					return;
				}
				rem_flat = rem_flat ->next;
			}
		}
		temp = temp ->next;	
	}
	if (temp -> next == head)
	{
		rem_flat = temp ->flat_list;
		while (rem_flat){
			if (rem_flat -> id == flat_id){
				rem_flat -> is_empty =1;
				rem_flat -> initial_bandwidth = 0;
				return;
			}
			rem_flat = rem_flat ->next;
		}
	}
}

int find_sum_of_max_bandwidths(Apartment* head){
	
	Apartment* tmp = head;
	int sum_of_max =0;
	
	if (head == NULL) return 0;
	
	while (tmp -> next != head){
		sum_of_max += tmp -> max_bandwidth;
		tmp = tmp -> next;
	}
	sum_of_max += tmp -> max_bandwidth; /*add last one*/
	
	return sum_of_max;
}


Apartment* merge_two_apartments(Apartment* head, char* apartment_name_1, char* apartment_name_2){
	
	Apartment* temp1 = head, *temp2 =head,*tail =head, *first =NULL, *sec =NULL;
	Flat* added, *moved;
	int a =0;
	
	while (tail -> next != head){
		tail = tail -> next;
	}
	while (!(helper1(temp1 ->next -> name,apartment_name_1))){
		temp1 = temp1 -> next;
	}
	while (!(helper1(temp2 -> next -> name, apartment_name_2))){
		temp2 = temp2 -> next;
	}
	
	temp1 -> next -> max_bandwidth += temp2 -> next -> max_bandwidth;
	first = temp1 -> next;
	sec = temp2->next;
	added = first -> flat_list; /*eklenen*/
	moved = sec -> flat_list; /*tasinan*/

	if (head == sec) a =1;
	
	
	if (added == NULL && moved == NULL){
		temp2 -> next= sec -> next;
		sec -> next = NULL; 
		free(sec);
	}
	
	
	else if (moved == NULL && added != NULL){
		temp2 -> next = sec ->next;
		sec -> next = NULL;
		free(sec);
	}
	
	else if(moved != NULL && added == NULL){
		first ->flat_list = moved;
		temp2 -> next = sec ->next;
		sec -> next = NULL;
		free(sec);
	}
	
	else if ((added != NULL) && (moved != NULL)){
		while (added -> next != NULL){
			added = added -> next;
		}	
		added -> next = moved;
		moved -> prev = added;
		temp2 -> next= sec -> next;
		sec -> next = NULL; 
		free(sec);
	}
	
	if (a) return (tail->next);
	
	return head;
}

int flat_search(Flat* headd, int flat_id){
	Flat* temp = headd;
	while(temp){
		if (temp->id ==flat_id) return 1;
		temp =temp->next;
	}
	return 0;
}

void relocate_flats_to_same_apartment(Apartment* head, char* new_apartment_name, int flat_id_to_shift, int* flat_id_list){
	
	int i=0;
	Apartment* temp = head, *apart =head;
	Flat* pre=NULL, *temp1 = NULL;
	Flat* now=apart -> flat_list;
	
	while(temp -> next != head){
		if(helper1(temp -> name, new_apartment_name))
			break;
		temp =temp ->next;
	}
	
	while(*(flat_id_list+i) != '\0'){
		apart =head;
		for(;;){
			if ((apart ->flat_list !=NULL) && flat_search(apart ->flat_list,*(flat_id_list+i)))
				break;
			apart = apart ->next;
		}

		now=apart -> flat_list;

		if((now->id) == flat_id_list[i]){
			temp1=now;
			apart ->flat_list = now ->next;
			if (apart->flat_list){
				apart ->flat_list -> prev = NULL;
			}
		}

		else{
			for(;;){
				if( (now -> id) == flat_id_list[i]){
					temp1=now;
					pre->next= now ->next;
					if(pre ->next !=NULL){
						(pre->next)->prev =pre;
					}
					break;
				}
				pre =now;
				now = now->next;	
			}
		}
		
		now = temp -> flat_list;
		if(now->id == flat_id_to_shift){
			temp1->prev =NULL;
			temp1->next =now;
			temp->flat_list = temp1;
			(temp1->next)->prev =temp1;
		}
		
		else{
			for(;;){
				if ((now->id) == flat_id_to_shift){
					pre->next=temp1;
					temp1->prev=pre;
					temp1->next=now;
					if (now) now->prev=temp1;
					break;
				}
				pre = now;
				now = now ->next;
			}
		}
		
		apart->max_bandwidth -= temp1->initial_bandwidth;
		temp->max_bandwidth += temp1->initial_bandwidth;
		
		i++;
	}
}















