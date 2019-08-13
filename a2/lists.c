#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lists.h"
#include <errno.h>

int remove_trans(Group *group, const char *user_name);
Xct *find_prev_xct(Group *group, const char *user_name);

/* Add a group with name group_name to the group_list referred to by
* group_list_ptr. The groups are ordered by the time that the group was
* added to the list with new groups added to the end of the list.
*
* Returns 0 on success and -1 if a group with this name already exists.
*
* (I.e, allocate and initialize a Group struct, and insert it
* into the group_list. Note that the head of the group list might change
* which is why the first argument is a double pointer.)
*/
int add_group(Group **group_list_ptr, const char *group_name) {
	/* make the group object
	*/
	Group *new_group = (Group*) malloc(sizeof(Group));
	/* check for malloc error if there is exit
	*/
	if (new_group == NULL){
		perror("malloc");
		exit(1);
		return (-1);}
		// malloc space for the name
	new_group -> name = malloc(strlen(group_name)+1);
	// copy the name into the parameter
	strcpy(new_group -> name,group_name);
	// set the rest to null
	new_group -> users = NULL;
	new_group -> xcts = NULL;
	new_group -> next = NULL;
	// make a pointer to add to groups
	Group* curr_group = *group_list_ptr;
	// if there are no groups it is the first group
	if (curr_group == NULL){
			*group_list_ptr = new_group;
			return 0;}
	//otherwise, it is the last item in the list of groups
	while (curr_group != NULL){
		if (strcmp(curr_group -> name, group_name) == 0 ){
			return (-1);}
		else if (curr_group-> next == NULL){
			curr_group-> next = new_group;
			return 0;
		}
		curr_group = curr_group-> next;
	}
		return 0;
		
}
/* Print to standard output the names of all groups in group_list, one name
*  per line. Output is in the same order as group_list.
*/
void list_groups(Group *group_list) {
	// make a pointer to the group list
	Group* curr_group = group_list;
	// if there are no groups in it then return the print line statement
	if (curr_group == NULL){
		printf("The group list is empty\n");
	return ;}
	//otherwise go through the list and print all their names
	printf("The groups in the list are:\n");
	while (curr_group != NULL){
		printf("%s\n", curr_group-> name);
		curr_group = curr_group -> next;
		}
}

/* Search the list of groups for a group with matching group_name
* If group_name is not found, return NULL, otherwise return a pointer to the
* matching group list node.
*/
Group *find_group(Group *group_list, const char *group_name) {
	// make the pointer to the group
    Group* list_of_groups = group_list;
	//if the list is empty, return NULL
	if (list_of_groups == NULL){
		return NULL;}
	// otherwise check each groups' name. 
	while (list_of_groups != NULL){
		// if the names match then return the group
		if (strcmp(list_of_groups -> name, group_name) == 0 ){
			return list_of_groups;}
		list_of_groups = list_of_groups-> next;
	}
	return NULL;
}

/* Add a new user with the specified user name to the specified group. Return zero
* on success and -1 if the group already has a user with that name.
* (allocate and initialize a User data structure and insert it into the
* appropriate group list)
*/
int add_user(Group *group, const char *user_name) {
	// make a user object
	User *user;	
	// malloc space for the user object
	user = (User*) malloc(sizeof(User));
	if (user == NULL){
		perror("malloc");
		exit(1);
		return (-1);}
	//malloc space for the name
	user -> name = malloc(strlen(user_name) + 1);
	if (user-> name == NULL){
	perror("malloc");
		exit(1);
		return (-1);}
	// copy the name to the parameter
	strcpy(user -> name,user_name);
	// set the balance to 0
	// the next user to none
	user -> balance = 0;
	user -> next = NULL;
	// make a pointer to the group's user list
	User *user_list = group -> users;
	// if the group is empty return fail
	if (group == NULL){
	return -1;}
	// if the userlist is empty, set the user to be the first one and return 0
	if (user_list == NULL){
	
	group -> users = user;
	return 0;
	}
	// otherwise the list is not empty and the grouplist existsso add the user to the front of the list
	else{
			//check to see if the user was created already
		if (find_prev_user(group, user_name) != NULL){
		return -1;
	}
		user -> next = user_list;
		group -> users= user;
		return 0;}
	return 0;
}


/* Remove the user with matching user and group name and
* remove all her transactions from the transaction list.
* Return 0 on success, and -1 if no matching user exists.
* Remember to free memory no longer needed.
* (Wait on implementing the removal of the user's transactions until you
* get to Part III below, when you will implement transactions.)
*/
int remove_user(Group *group, const char *user_name) {

	//make a pointer to the group user list
	User *user_list = group -> users;
	// if the list is empty return fail
	if (user_list == NULL){
	return -1;}
	// check to see if the user is in the list if not return fail
	User *prev_user = find_prev_user(group, user_name);
	if (prev_user == NULL){
		return -1;}

	
		// if he is the first person in the list move the list to point to the second and free him

	if (strcmp(prev_user-> name, user_name) == 0){
		group -> users = prev_user -> next;
		free(prev_user);}
		//otherwise remove all his pointers and repoint the others
	else if (strcmp(prev_user -> next -> name, user_name) == 0){
		User *delete_user = prev_user -> next;	
		prev_user -> next = prev_user -> next ->next;
		free(delete_user);
        }
	remove_xct(group, user_name);

    return 0;}

/* Print to standard output the names of all the users in group, one
* per line, and in the order that users are stored in the list, namely
* lowest payer first.
*/
void list_users(Group *group) {
	// make a ptr to the user list
	User *user_list = group -> users;
	// if it is empty return the print line
	if (user_list == NULL){
	printf("There are no users in this group\n");}
	//otherwise go through the list and print all user's names
	else{
		printf("the users in the group are:\n");
		while (user_list != NULL){
		printf("%s\n", user_list -> name);
		user_list = user_list -> next;}
	}
}
/* Print to standard output the balance of the specified user. Return 0
* on success, or -1 if the user with the given name is not in the group.
*/
int user_balance(Group *group, const char *user_name) {

	// if the group is empty then return fail
	if (group == NULL){
		return -1;}
	// creat a ptr to the group list
	User *user_list = group -> users;
	// if the list is empty then return fail
	if (user_list == NULL){
		return -1;}
	// otherwise, return a fail and get the previous user to the one we want
	else{
		User *prev_user = find_prev_user(group, user_name);
		// if one does not exist return fail
		if (prev_user == NULL){
		return -1;
		}
		else if (prev_user != NULL){
			if (strcmp(prev_user-> name, user_name)== 0){
			printf("%f\n", prev_user -> balance);}
			else{
			double user_balance = prev_user -> next -> balance;
			printf("%f\n", user_balance);
		}
	}
	return 0;
	}
}
/* Print to standard output the name of the user who has paid the least
* If there are several users with equal least amounts, all names are output.
* Returns 0 on success, and -1 if the list of users is empty.
* (This should be easy, since your list is sorted by balance).
*/
int under_paid(Group *group) {
	// if the group is empty return fail
	if(group == NULL){
		return -1;
	}
	// if the user list is empty return fail
	User *user_list = group -> users;
	if (user_list == NULL){
		return -1;
	}
	// otherwise
	else{
		// get the lowest balance
		double lowest_bal = user_list -> balance;
		// while the current user is not null and the balance is equal to lowest balance print the names
		while (user_list != NULL && user_list -> balance == lowest_bal){
			printf("%s\n", user_list -> name);
			user_list = user_list -> next;
			}
	return 0;
	}
}

/* Return a pointer to the user prior to the one in group with user_name. If
* the matching user is the first in the list (i.e. there is no prior user in
* the list), return a pointer to the matching user itself. If no matching user
* exists, return NULL.
*
* The reason for returning the prior user is that returning the matching user
* itself does not allow us to change the user that occurs before the
* matching user, and some of the functions you will implement require that
* we be able to do this.
*/
User *find_prev_user(Group *group, const char *user_name) {
	//if the group is empty, fail
	if(group == NULL){
	return NULL;}
	// if the user list is empty return fail
    else if (group -> users == NULL){
        return NULL;}
	//if the list is not empty
    else if (group -> users != NULL){
		// get the pointer to the first item
        User *curr_user = group -> users;
		// if the first is the desired user return the first item
		if (strcmp(curr_user ->name, user_name) == 0){
			return curr_user;
		}
		//otherwise loop through the list
        while (curr_user ->next != NULL){
			// if the next person's is the right one return the current one
            if (strcmp(curr_user -> next -> name, user_name) == 0){
                return curr_user;
                }
            curr_user = curr_user -> next;}
            }
		return NULL;
    }

/* Add the transaction represented by user_name and amount to the appropriate
* transaction list, and update the balances of the corresponding user and group.
* Note that updating a user's balance might require the user to be moved to a
* different position in the list to keep the list in sorted order. Returns 0 on
* success, and -1 if the specified user does not exist.
*/
int add_xct(Group *group, const char *user_name, double amount) {

	//if the group is empty return fail
	if (group == NULL){
		return -1;
	}

	// if the users are empty return fail
	if (group -> users == NULL){
		return -1;
	}

	// check to see if the name is in the lsit
	User *prev_user = find_prev_user(group, user_name);
	// if it is not return fail
	if (prev_user == NULL){
		return -1;
	}

	// make a xct item through malloc
    Xct *transact = (Xct*) malloc(sizeof(Xct));
	// if malloc failed then return fail
	if (transact == NULL){
		perror("malloc");
		exit(1);
		return (-1);}
	// malloc the name
	transact -> name = malloc(strlen(user_name) +1);
	// if malloc failed returnn fail 
	if (transact -> name == NULL){
		perror("malloc");
		exit(1);
		return (-1);}
	// set the next to null and copy the name into the parameter also set the amount	printf("f5\n");
	transact -> next = NULL;
	strcpy(transact -> name, user_name);
	transact -> amount = amount;

	// if the transaction list is empty then it is the first traansaction
	if (group -> xcts == NULL){
		group -> xcts = transact;
	}
	// otherwise set it to be the first transaction
	else if (group -> xcts != NULL){
		transact -> next = group -> xcts;
		group -> xcts = transact;
	}

	// update the transaction
	if (strcmp(prev_user -> name, user_name) == 0){
		prev_user -> balance += amount;
		}
	else{
	prev_user -> next -> balance += amount;}

	User *temp;

	if (group -> users -> next == NULL){
		return 0;
	}

	if (strcmp(prev_user -> name, user_name) == 0){
		temp = prev_user;
	group -> users = temp -> next;
	temp -> next = NULL;}
	else{
		temp = prev_user-> next;
	prev_user -> next = prev_user -> next -> next;
	temp -> next = NULL;}
	User *user_list = group -> users;
	while (user_list != NULL){
		/*
		if the balance of the current user less than the balance of the current user keep going through the list
		if the user_balance is greater than the balance we have in the node we moved put the moved user in front of the current user
		*/
		if (user_list -> balance >= temp -> balance){
			User *prev_prev_user = find_prev_user(group, user_list -> name);
			temp -> next = prev_prev_user -> next;
			prev_prev_user -> next = temp;
			return 0;}
		// if we reached the end of the list add the node to the end
		if (user_list -> next == NULL){
		user_list -> next = temp;
		return 0;
		}
		user_list = user_list -> next;
		}
		/*
		find the prev user of the current item if its balance is equal to the transaction balance
		then set the user who's balance we added to be between the prev user and the current user. 
		*/
		return 0;
	}

/* Print to standard output the num_xct most recent transactions for the
* specified group (or fewer transactions if there are less than num_xct
* transactions posted for this group). The output should have one line per
* transaction that prints the name and the amount of the transaction. If
* there are no transactions, this function will print nothing.
*/
void recent_xct(Group *group, long nu_xct) {
	if (group == NULL){
		printf("Invalid Group");
		return;
	}
	if (group -> xcts == NULL){
		printf("No transactions available");
	}
	if (group -> xcts != NULL){
		Xct *xct_list = group -> xcts;
		int i = 0;
		while (xct_list  != NULL && i <= nu_xct){
			printf(" Name: %s\n  Transaction Amount: %f\n" , xct_list -> name, xct_list -> amount);
			xct_list = xct_list -> next;
		}
	}
	return;
			
}

/* Remove all transactions that belong to the user_name from the group's
* transaction list. This helper function should be called by remove_user.
* If there are no transactions for this user, the function should do nothing.
* Remember to free memory no longer needed.
*/
void remove_xct(Group *group, const char *user_name) {
	int i = 0;
	if (group -> xcts == NULL){
	return;}
	while (i == 0){
			 i = remove_trans(group, user_name);
	}
}
int remove_trans(Group *group, const char *user_name) {
	//make a pointer to the group user list
	Xct *xct_list = group -> xcts;
	// check to see if the user is in the list if not return fail
	// if the list is empty return fail
	if (xct_list == NULL){
		return -1;
	}
		// if the first item is a traget item return true after removing it
	if (strcmp(xct_list -> name, user_name) == 0){
		Xct *temp = group -> xcts;
		group -> xcts = group -> xcts -> next;
	free(temp);
	return 0;
	}
	// check to see if the user is in the list if not return fail
	Xct *prev_xct = find_prev_xct(group, user_name);
	if (prev_xct == NULL){
        return -1;
	}
	// if he does exist
    else if (prev_xct != NULL){
		//otherwise remove all his pointers and repoint the others
		if (strcmp(prev_xct -> next -> name, user_name) == 0){
			Xct *delete_xct = prev_xct -> next;
			prev_xct -> next = delete_xct ->next;
			delete_xct -> next = NULL;
			free(delete_xct);
			return 0;
        }
    }
    return 0;}
	
Xct *find_prev_xct(Group *group, const char *user_name) {
	//if the group is empty, fail
	if(group == NULL){
	return NULL;}
	// if the user list is empty return fail
    else if (group -> xcts == NULL){
        return NULL;}
	//if the list is not empty
    else if (group -> xcts != NULL){
		// get the pointer to the first item
        Xct *curr_xct = group -> xcts;
		// if the first is the desired xct return the first item
		if (strcmp(curr_xct ->name, user_name) == 0){
			return curr_xct;
		}
		//otherwise loop through the list
        while (curr_xct ->next != NULL){
			// if the next person's is the right one return the current one
            if (strcmp(curr_xct -> next -> name, user_name) == 0){
                return curr_xct;
                }
            curr_xct = curr_xct -> next;}
            }
		return NULL;
    }