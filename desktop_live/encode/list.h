#ifndef __C_LIST_H
#define __C_LIST_H

typedef unsigned char     u8;
typedef unsigned short    u16;
typedef unsigned int      u32;

#ifndef size_t
//typedef unsigned long     size_t;
#endif

#define offsetof(TYPE, MEMBER)   ((size_t) &((TYPE *)0)->MEMBER)

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:    the pointer to the member.
 * @type:    the type of the container struct this is embedded in.
 * @member:    the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) (type *)((char *)ptr -offsetof(type,member))

/*
 * These are non-NULL pointers that will result in page faults
 * under normal circumstances, used to verify that nobody uses
 * non-initialized list entries.
 */
#define LIST_POISON1  ((void *) 0x00100100)
#define LIST_POISON2  ((void *) 0x00200200)

struct list_head {
    struct list_head *next, *prev;
};

/**
 * list_entry - get the struct for this entry
 * @ptr:    the &struct list_head pointer.
 * @type:    the type of the struct this is embedded in.
 * @member:    the name of the list_struct within the struct.
 */
#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)
    

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)

static __inline void INIT_LIST_HEAD(struct list_head *list)
{
    list->next = list;
    list->prev = list;
}

/**
 * list_for_each    -    iterate over a list
 * @pos:    the &struct list_head to use as a loop counter.
 * @head:    the head for your list.
 */
#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * list_for_each_r    -    iterate over a list reversely
 * @pos:    the &struct list_head to use as a loop counter.
 * @head:    the head for your list.
 */
#define list_for_each_r(pos, head) \
    for (pos = (head)->prev; pos != (head); pos = pos->prev)    

/*
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static __inline void __list_add(struct list_head *new_head,
                  struct list_head *prev,
                  struct list_head *next)
{
    next->prev = new_head;
    new_head->next = next;
    new_head->prev = prev;
    prev->next = new_head;
}

/**
 * list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static __inline void list_add(struct list_head *new_head, struct list_head *head)
{
    __list_add(new_head, head, head->next);
}

/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static __inline void list_add_tail(struct list_head *new_head, struct list_head *head)
{
    __list_add(new_head, head->prev, head);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static __inline void __list_del(struct list_head * prev, struct list_head * next)
{
    next->prev = prev;
    prev->next = next;
}

/**
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty on entry does not return true after this, the entry is
 * in an undefined state.
 */
static __inline void list_del(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    entry->next = (struct list_head *)LIST_POISON1;
    entry->prev = (struct list_head *)LIST_POISON2;
}


/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
static __inline int list_empty(const struct list_head *head)
{
    return head->next == head;
}


static __inline void __list_splice(struct list_head *list,
                 struct list_head *head)
{
    struct list_head *first = list->next;
    struct list_head *last = list->prev;
    struct list_head *at = head->next;

    first->prev = head;
    head->next = first;

    last->next = at;
    at->prev = last;
}

/**
 * list_splice - join two lists
 * @list: the new list to add.
 * @head: the place to add it in the first list.
 */
static __inline void list_splice(struct list_head *list, struct list_head *head)
{
    if (!list_empty(list))
        __list_splice(list, head);
}


#endif // __C_LIST_H