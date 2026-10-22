/*
 * A376301  a(n) is the smallest 7-smooth number ending with n, or -1 if there
 *          is none.
 * https://oeis.org/A376301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 21, i16 n <= 65, i32 n <= 106, i64 n <= 222, i128 none,
 *            big any n
 * Helpers:   seqt_max
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(n) = the smallest 7‑smooth number whose decimal representation ends with n,
 *   or -1 if no such number exists.
 *
 * The computation is a breadth‑first search (Dijkstra) over residues modulo 10^MAX_D.
 * For each residue r we store the minimal 7‑smooth value that has that residue.
 * Then for each suffix length d (1 ≤ d ≤ MAX_D) we keep the minimal value
 * among all residues whose remainder modulo 10^d equals the suffix.
 * The answer for a(n) (where n has d decimal digits) is lookup[ d * MOD + n ].
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seqt_max.h"

#define MAX_D 6                               /* maximum number of suffix digits handled */
#define MOD   1000000u                        /* 10^MAX_D */

/* primes whose powers are allowed in a 7‑smooth number */
static const int primes[4] = {2, 3, 5, 7};

/* number of decimal digits of x (x ≥ 0, idx_t is an unsigned 64‑bit type) */
static uint32_t digits(idx_t x)
{
    if (x == 0) return 1;
    uint32_t d = 0;
    while (x != 0) { d++; x /= 10; }
    return d;
}

/*---------------  SIMPLE BINARY‑HEAP ---------------*/
typedef struct {
    unsigned res;   /* residue modulo MOD */
    seq_t    val;   /* 7‑smooth value */
} node_t;

static node_t *heap = NULL;
static size_t heap_size = 0, heap_cap = 0;

static void heap_push(node_t n)
{
    if (heap_size == heap_cap) {
        size_t new_cap = heap_cap ? heap_cap * 2 : 16;
        heap = (node_t *)realloc(heap, new_cap * sizeof(node_t));
        heap_cap = new_cap;
    }
    size_t i = heap_size++;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].val <= n.val) break;
        heap[i] = heap[p];
        i = p;
    }
    heap[i] = n;
}

static node_t heap_pop(void)
{
    node_t res = heap[0];
    heap[0] = heap[--heap_size];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t smallest = i;
        if (l < heap_size && heap[l].val < heap[smallest].val) smallest = l;
        if (r < heap_size && heap[r].val < heap[smallest].val) smallest = r;
        if (smallest == i) break;
        node_t tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
    return res;
}

/*---------------  GLOBAL TABLES ---------------*/
static seq_t *min_val   = NULL;            /* minimal 7‑smooth for each residue modulo MOD */
static bool  *known     = NULL;            /* whether min_val[res] has been set */
static seq_t *ans_table = NULL;            /* flattened answer table: [d][n] -> value */

/* initialize all tables on the first call */
static void init_tables(void)
{
    /* allocate tables */
    min_val   = (seq_t *)malloc(MOD * sizeof(seq_t));
    known     = (bool *)calloc(MOD, sizeof(bool));
    ans_table = (seq_t *)malloc((MAX_D + 1) * MOD * sizeof(seq_t));

#ifdef __cplusplus
    /* value‑initialize seq_t elements in C++ */
    for (size_t i = 0; i < MOD; ++i)       new(&min_val[i]) seq_t();
    for (size_t i = 0; i < (MAX_D + 1) * MOD; ++i) new(&ans_table[i]) seq_t();
#endif

    /* mark all answers as -1 */
    for (size_t i = 0; i < (MAX_D + 1) * MOD; ++i)
        ans_table[i] = (seq_t)-1;

    /* Dijkstra: compute minimal 7‑smooth number for every residue modulo MOD */
    heap = NULL;
    heap_size = 0;
    heap_cap = 0;
    heap_push((node_t){1 % MOD, (seq_t)1});

    const seq_t maxval = seqt_max();

    while (heap_size > 0) {
        node_t cur = heap_pop();
        unsigned r = cur.res;
        if (known[r]) continue;

        known[r] = true;
        min_val[r] = cur.val;

        for (int k = 0; k < 4; ++k) {
            int p = primes[k];
            unsigned nr = (unsigned)(((unsigned long long)r * p) % MOD);
            if (known[nr]) continue;

            /* overflow guard for fixed‑size seq_t */
            if (sizeof(seq_t) <= 8) {
                if (cur.val > maxval / p) continue;
            }
            seq_t prod = cur.val * p;
            heap_push((node_t){nr, prod});
        }
    }

    /* build answer tables for suffix lengths 1 … MAX_D */
    for (int d = 1; d <= MAX_D; ++d) {
        size_t base = 1;
        for (int i = 0; i < d; ++i) base *= 10;
        for (size_t r = 0; r < MOD; ++r) {
            if (!known[r]) continue;
            seq_t val = min_val[r];
            size_t ns = r % base;
            seq_t *slot = &ans_table[d * MOD + ns];
            if (*slot == (seq_t)-1 || val < *slot) *slot = val;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    static bool ready = false;
    if (!ready) {
        init_tables();
        ready = true;
    }

    uint32_t d = digits(n);
    if (d > MAX_D) return (seq_t)-1;   /* no data for longer suffixes */

    return ans_table[d * MOD + (size_t)n];
}
