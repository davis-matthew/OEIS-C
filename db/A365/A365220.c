/*
 * A365220  Each term is a "Go flat integer" (GFI), but a(n) + a(n+1) is
 *          always a "Go up integer" (GUI). More details in the Comments
 *          section.
 * https://oeis.org/A365220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 1; for n>1, a(n) is the smallest unused Go flat integer b
 *            such that a(n‑1)+b is a Go up integer
 * Range:     u8 n <= 19, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   is_gfi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_gfi.h"

/* BST node for keeping track of used GFI numbers */
struct Node {
    seq_t key;
    struct Node *left;
    struct Node *right;
};

/* allocate a new node */
static struct Node *node_alloc(seq_t key)
{
    struct Node *p = (struct Node *)malloc(sizeof(*p));
    if (!p) abort();
    p->key = key;
    p->left = p->right = NULL;
    return p;
}

/* insert key into BST; return new root */
static struct Node *bst_insert(struct Node *root, seq_t key)
{
    if (!root) return node_alloc(key);
    struct Node *cur = root, *parent = NULL;
    int cmp = 0;
    while (cur) {
        if (key == cur->key) return root;          /* already present */
        parent = cur;
        if (key < cur->key) { cur = cur->left; cmp = -1; }
        else { cur = cur->right; cmp = 1; }
    }
    if (cmp < 0) parent->left = node_alloc(key);
    else parent->right = node_alloc(key);
    return root;
}

/* search key in BST */
static bool bst_find(struct Node *root, seq_t key)
{
    while (root) {
        if (key == root->key) return true;
        else if (key < root->key) root = root->left;
        else root = root->right;
    }
    return false;
}

/* helper: true if x is a GUI (at least two digits and last digit > first digit) */
static bool is_gui(seq_t x)
{
    if (x < 10) return false;
    seq_t last = x % 10;
    seq_t tmp = x;
    while (tmp >= 10) tmp /= 10;
    seq_t first = tmp;
    return last > first;
}

/* dynamic array of sequence terms */
static seq_t *seq_arr = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* BST of used GFI numbers */
static struct Node *used_root = NULL;

/* initialize sequence with a(1)=1 */
static void seq_init(void)
{
    if (seq_arr) return;                       /* already initialized */
    seq_cap = 16;
    seq_arr = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    if (!seq_arr) abort();
    seq_arr[0] = (seq_t)1;
    seq_len = 1;
    used_root = bst_insert(used_root, (seq_t)1);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* offset is 1; value for 0 is irrelevant */
    seq_init();                                 /* ensure array is ready */

    while (seq_len < (size_t)n) {
        seq_t prev = seq_arr[seq_len - 1];
        seq_t cand = 0;
        for (;;) {
            if (!is_gfi(cand)) { ++cand; continue; }
            if (bst_find(used_root, cand)) { ++cand; continue; }
            seq_t sum = prev + cand;
            if (is_gui(sum)) break;
            ++cand;
        }
        /* append cand to sequence */
        if (seq_len == seq_cap) {
            seq_cap *= 2;
            seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
            if (!seq_arr) abort();
        }
        seq_arr[seq_len++] = cand;
        used_root = bst_insert(used_root, cand);
    }
    return seq_arr[n - 1];
}
