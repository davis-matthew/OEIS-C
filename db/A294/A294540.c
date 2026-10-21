/*
 * A294540  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-2) + 2*n - 1, where a(0) = 1, a(1) = 2, b(0) = 3.
 * https://oeis.org/A294540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + a(n-2) + b(n-2) + 2*n - 1
 *   a(0) = 1, a(1) = 2, b(0) = 3, b(1) = 4
 *   b(n) is the least positive integer not used by
 *   a(k) or b(k) for all k < n.
 * This implementation memoises both sequences and keeps a
 * hash table of used numbers so that b(n) is found in O(1).
 */

#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build system. */
static seq_t *A = NULL;           /* a-values */
static seq_t *B = NULL;           /* b-values */
static size_t A_cap = 0;          /* capacity of A and B arrays */
static idx_t  max_comp = 1;       /* highest n whose a,b are known (after init) */

/* Hash table for values already used in a() or b() */
#define HASH_START 131101
static seq_t *hash_tab = NULL;
static unsigned char *hash_used = NULL;
static size_t hash_cap = 0;
static size_t hash_cnt = 0;
static seq_t next_candidate = 5;  /* first value to test for b(n) after init */

/* forward declarations */
static void hash_init(void);
static void hash_insert(seq_t x);
static bool hash_contains(seq_t x);
static void hash_grow(void);
static inline size_t hash_index(seq_t x);

/* ------------------------------------------------------------------ */
static inline size_t hash_index(seq_t x)
{
    return (size_t)(x % (seq_t)hash_cap);
}

static void hash_init(void)
{
    hash_cap = HASH_START;
    hash_cnt = 0;
    hash_tab  = (seq_t*)calloc(hash_cap, sizeof(seq_t));
    hash_used = (unsigned char*)calloc(hash_cap, sizeof(unsigned char));
    if (!hash_tab || !hash_used) abort();
}

static void hash_insert(seq_t x)
{
    if (hash_cap == 0) hash_init();

    /* Grow to maintain load factor < 0.75 */
    if ((hash_cnt + 1) * 4 > hash_cap * 3) hash_grow();

    size_t h = hash_index(x);
    while (hash_used[h]) {
        if (hash_tab[h] == x) return; /* already present */
        h = (h + 1) % hash_cap;
    }
    hash_tab[h] = x;
    hash_used[h] = 1;
    hash_cnt++;
}

static bool hash_contains(seq_t x)
{
    if (hash_cap == 0) return false;
    size_t h = hash_index(x);
    while (hash_used[h]) {
        if (hash_tab[h] == x) return true;
        h = (h + 1) % hash_cap;
    }
    return false;
}

static void hash_grow(void)
{
    size_t new_cap = hash_cap * 2;
    seq_t *new_tab  = (seq_t*)calloc(new_cap, sizeof(seq_t));
    unsigned char *new_used = (unsigned char*)calloc(new_cap, sizeof(unsigned char));
    if (!new_tab || !new_used) abort();

    for (size_t i = 0; i < hash_cap; ++i) {
        if (hash_used[i]) {
            seq_t val = hash_tab[i];
            size_t h = (size_t)(val % (seq_t)new_cap);
            while (new_used[h]) h = (h + 1) % new_cap;
            new_tab[h] = val;
            new_used[h] = 1;
        }
    }

    free(hash_tab);
    free(hash_used);
    hash_tab  = new_tab;
    hash_used = new_used;
    hash_cap  = new_cap;
}

/* ------------------------------------------------------------------ */
/*                       MAIN FUNCTION DEFINITION                      */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Initial allocation for a() and b() */
    if (A_cap == 0) {
        A_cap = 8;
        A = (seq_t*)malloc(A_cap * sizeof(seq_t));
        B = (seq_t*)malloc(A_cap * sizeof(seq_t));
        if (!A || !B) abort();

        /* initialise first terms */
        A[0] = (seq_t)1;
        A[1] = (seq_t)2;
        B[0] = (seq_t)3;
        B[1] = (seq_t)4;                 /* first complementary value */

        /* initialise hash table with used numbers: 1,2,3,4 */
        hash_insert((seq_t)1);
        hash_insert((seq_t)2);
        hash_insert((seq_t)3);
        hash_insert((seq_t)4);

        next_candidate = 5;                /* start searching after 4 */
        max_comp = 1;                      /* a(0) and a(1) known */
    }

    /* enlarge a()/b() storage if necessary */
    if ((size_t)n + 1 > A_cap) {
        size_t new_cap = A_cap ? A_cap : 4;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        A = (seq_t*)realloc(A, new_cap * sizeof(seq_t));
        B = (seq_t*)realloc(B, new_cap * sizeof(seq_t));
        if (!A || !B) abort();
        A_cap = new_cap;
    }

    /* if already computed, return the stored value */
    if (n <= max_comp) return A[n];

    /* compute values from max_comp+1 up to n */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        /* compute a(i) for i>=2 */
        if (i > 1) {
            seq_t term = A[i-1] + A[i-2] + B[i-2] +
                         (((seq_t)i * (seq_t)2) - (seq_t)1);
            A[i] = term;
            hash_insert(term);
        }

        /* compute b(i) for i>=1 (b[0] already set) */
        while (true) {
            if (!hash_contains(next_candidate)) {
                B[i] = next_candidate;
                hash_insert(next_candidate);
                next_candidate++;   /* next search starts after this number */
                break;
            }
            next_candidate++;
        }
    }

    max_comp = n;
    return A[n];
}
