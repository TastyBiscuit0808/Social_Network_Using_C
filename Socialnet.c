// Hey coder, this was one of my CS assignments.

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include<stdbool.h>
#define MAX 100

typedef struct node 
{
    int id; //ID of user
    int numfren; //number of friends of user
    char name[MAX]; //name of user
    int* friends; //friends of user as an array
    struct node* right;  //user to the right
    struct node* left; //user to the left
} node;

struct node* retUser(char str[MAX])
{
    char name[MAX];
    char ID[MAX];
    char strfriends[MAX];

    //copied ID
    char*token=strtok(str,",");
    strcpy(ID,token);

    //copied Name
    token=strtok(NULL,",");
    strcpy(name,token);

    //copied friends' ID
    token=strtok(NULL,",");
    strcpy(strfriends,token);
    //printf("%s\n",(strfriends));

    //creating user nodes
    int id=atoi(ID);
    struct node *user = malloc(sizeof(struct node));
    user->id=id;
    user->friends=(int*)malloc(MAX * sizeof(int));
    strcpy(user->name,name);


    //adding user's friend's IDs
    token = strtok(strfriends,"|");        
    int i=0;
    while( token != NULL ) 
    {

        int temp=atoi(token);
        user->friends[i]=temp;
        i++;
        token = strtok(NULL,"|");
    }
    user->numfren=i;
    if(i==0){user->friends[i]=-1;}
    return user;
}

//search for user with id=key
struct node* search(int key, struct node *users)
{   
    node *cur = users;
    
    if(cur == NULL)
    {
        return NULL;
    }
    if(cur->id == key)
    {
        return cur;
    }
    else
    {
        if(users->id<key)
        {
            return search(key, users->right);
        }
        else
        {
            return search(key, users->left);
        }
    }


}


struct node*refineUser(struct node*user, struct node *users)
{
    user->left = NULL;
    user->right = NULL;
    while(search(user->id, users)!=NULL) 
    {
        user->id++;
    }

    int i = 0;
    node* temp;
    while(i<user->numfren)
    {
        temp = search(user->friends[i], users);
        if(temp == NULL)
        {
            int j = i;
            while(j<user->numfren-1)
            {
                user->friends[j] = user->friends[j+1];
                j++;
            }
            user->numfren--;
        }
        else
        {
            temp->friends[temp->numfren] = user->id;
            temp->numfren++;
            i++;
        }
    }

    if(user->numfren == 0)
    {
        user->friends[0] = -1; 
    }
    return(user);
}

//insert user with id
struct node* insertUser(struct node*root,int id,struct node*user)
{
    if(root == NULL)
    {
        root = user;
    }   
    else
    {
        if(id>root->id)
        {
            root->right = insertUser(root->right,id, user);
        }
        else
        {
            root->left = insertUser(root->left,id,user);
        }
    }
    return(root);
     
}

//prints friends list
void friends(int id, struct node *users) 
{
    node *cur = search(id,users);
    if(cur == NULL)
    {
        return;
    }
    if(cur->numfren == 0)                       
    {    
        printf("-1\n");
    }
    for(int i =0;i<(cur->numfren);i++)
    {
        printf("%d\n",cur->friends[i]);
    }

}

//find child node with minimum value (inorder successor) - helper to delete node
struct node *minValueNode(struct node *node)
{
  
    if(node->right == NULL)
    {
        return NULL;
    }
    node = node->right;
    while(node->left!=NULL)
    {
        node = node->left;
    }
    return (node);
}

//deletes itself from its friend's nodes
struct node*deleteFriends(int key, struct node*users)
{
    
    node* temp = search(key, users);
    if(temp == NULL) return NULL;
    else
    {
        int i = 0;
        while(i<temp->numfren)
        {
            node* fren = search(temp->friends[i], users);
            if(fren!=NULL)
            {
                int j = 0;
                while(j<fren->numfren)
                {
                    if(fren->friends[j] != key) j++;
                    else
                    {
                        int k = j; 
                        while(k<fren->numfren)
                        {
                            fren->friends[k] = fren->friends[k+1];
                            k++;
                        }
                        fren->numfren--;
                    }

                }
            }
            i++;
        }
    }
    return users;
}

// Deleting a node
struct node *deleteNode(struct node *root, int key) 
{

    if(root == NULL) 
    {
        return root;
    }
    if(key>root->id)
    {
        root->right = deleteNode(root->right, key);
    }
    
    else if(key<root->id)
    {
        root->left = deleteNode(root->left, key);
    }
    
    else
    {
        if(root->left == NULL && root->right == NULL)
        {
            return NULL;
        }
        if(root->left== NULL)
        {
            node* temp = root->right;
            free(root);
            return temp;
        }
        if(root->right == NULL)
        {
            node* temp = root->left;
            free(root);
            return temp;
        }
       

        node* temp = minValueNode(root);
        root->id = temp->id;
        int i = 0;
        int* d = root->friends;
        root->friends = temp->friends;
        free(d);
        root->numfren = temp->numfren;
        strcpy(root->name, temp->name);
        root->right = deleteNode(root->right, temp->id); // deleting inorder successor
    }
    return root;
  
}

//Print USER's IDs in ascending order
void printInOrder(node* myusers) 
{
    if(myusers!=NULL)
    {
        printInOrder(myusers->left);
        printf("%d %s\n",myusers->id,myusers->name);    
        printInOrder(myusers->right);
    }
}

int main(int argc, char **argv)
{
    node *users=NULL;   
    while(1)
    {

        int opt, id;
        fflush(stdin);
        scanf("%d",&opt);
        char str[MAX];
        switch (opt)
        {
            case 1:
      
                scanf("%s",str);
                struct node*tbins=retUser(str);
                tbins=refineUser(tbins, users);
                users=insertUser(users,tbins->id,tbins);
                break;

            case 2:
           
                scanf("%d", &id);
                deleteFriends(id, users);
                users=deleteNode(users, id);
                break;

            case 3:
        
                scanf("%d", &id);
                node* result=search(id,users);
                if(result==NULL) 
                    printf("USER NOT IN TREE.\n");
                else{
                    printf("%d\n",result->id);
                }
                break;
                
            case 4:
                scanf("%d", &id);
                friends(id, users);
                break;

            case 5:
                printInOrder(users);
                break;

            case 6:
                exit(0);
                break;

            default: 
                printf("Wrong input! \n");
                break;
        }
    }
    return 0;
}
