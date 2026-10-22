/*
 * A388651  a(1) = 1, a(2) = 2; for n > 2, a(n) is the smallest unused
 *          positive number that has at least one prime exponent in its prime
 *          factorization that equals a prime exponent in the prime
 *          factorization of a(n-1).
 * https://oeis.org/A388651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=2; for n>2 a(n) is the smallest unused positive integer
   whose prime factorisation contains at least one exponent in common with a(n-1).
   The implementation builds, for every exponent e, a sorted list of numbers having
   that exponent, and keeps a pointer for each list.  A term is obtained by
   examining the lists for the exponents of the previous term and selecting the
   smallest unused candidate.  The lists are extended dynamically as needed. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_EXP 64                      /* maximum possible prime exponent for u64 */

/* --------- data structures --------- */
typedef struct {
    size_t size;                        /* number of elements stored   */
    size_t cap;                         /* allocated capacity          */
    size_t pos;                         /* next position to check      */
    uint64_t *data;                     /* list of numbers with this exp */
} ExpList;

/* --------- global state --------- */
static bool      _initialized = false;
static seq_t    *seq = NULL;            /* sequence values (1‑indexed)  */
static size_t    seq_cap = 0;           /* capacity of seq              */
static size_t    seq_size = 0;          /* highest index computed       */

static size_t    max_limit = 0;         /* maximum numeric value processed */
static uint8_t  *used = NULL;           /* bitmap of used numbers        */
static ExpList   exp_lists[MAX_EXP];    /* lists indexed by exponent     */

/* --------- helpers --------- */

/* ensure seq has capacity for at least 'cap' elements (1‑indexed) */
static void ensure_seq_capacity(size_t cap)
{
    if (cap <= seq_cap) return;
    seq_cap = cap * 2;
    seq = (seq_t *)realloc(seq, (seq_cap + 1) * sizeof(seq_t));
}

/* factorise n (64‑bit) and return distinct exponents in 'exps' */
static void factor_uint64(uint64_t n, uint16_t *exps, size_t *out_cnt)
{
    size_t cnt = 0;
    if (n <= 1) { *out_cnt = 0; return; }

    uint64_t m = n;
    uint64_t e = 0;
    if ((m & 1) == 0) {
        e = 0;
        while ((m & 1) == 0) { m >>= 1; e++; }
        exps[cnt++] = (uint16_t)e;
    }
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) { m /= p; e++; }
            exps[cnt++] = (uint16_t)e;
        }
    }
    if (m > 1) exps[cnt++] = 1;  /* remaining prime factor */
    *out_cnt = cnt;
}

/* add number 'num' to the exponent list 'exp' */
static void add_to_exp_list(uint16_t exp, uint64_t num)
{
    ExpList *L = &exp_lists[exp];
    if (L->cap == 0) {
        L->cap = 8;
        L->data = (uint64_t *)malloc(L->cap * sizeof(uint64_t));
    } else if (L->size == L->cap) {
        L->cap <<= 1;
        L->data = (uint64_t *)realloc(L->data, L->cap * sizeof(uint64_t));
    }
    L->data[L->size++] = num;
}

/* extend pre‑computed lists and used bitmap up to 'new_limit' */
static void extend_lists_up_to(size_t new_limit)
{
    if (new_limit <= max_limit) return;

    /* resize used bitmap */
    used = (uint8_t *)realloc(used, (new_limit + 1) * sizeof(uint8_t));
    memset(used + max_limit + 1, 0,
           (new_limit - max_limit) * sizeof(uint8_t));

    /* generate numbers from max_limit+1 to new_limit */
    uint16_t exps[32];
    size_t   exps_cnt;
    for (size_t num = max_limit + 1; num <= new_limit; ++num) {
        factor_uint64(num, exps, &exps_cnt);
        for (size_t i = 0; i < exps_cnt; ++i) {
            uint16_t e = exps[i];
            if (e < MAX_EXP)
                add_to_exp_list(e, num);
        }
    }
    max_limit = new_limit;
}

/* initialisation */
static void init(void)
{
    seq_cap = 64;
    seq = (seq_t *)malloc((seq_cap + 1) * sizeof(seq_t));
    seq[1] = (seq_t)1;
    seq[2] = (seq_t)2;
    seq_size = 2;

    for (int i = 0; i < MAX_EXP; ++i) {
        exp_lists[i].size = exp_lists[i].cap = exp_lists[i].pos = 0;
        exp_lists[i].data = NULL;
    }

    /* start with a modest limit so that we do not grow immediately */
    extend_lists_up_to(1000);
    used[1] = 1;
    used[2] = 1;

    _initialized = true;
}

/* --------- main function --------- */
seq_t a(idx_t n)
{
    if (!_initialized) init();

    if (n <= 0) return (seq_t)0;                /* undefined for non‑positive indices */

    if ((size_t)n <= seq_size)                  /* already computed */
        return seq[(size_t)n];

    /* compute missing terms up to n */
    while ((size_t)n > seq_size) {
        seq_t prev_val = seq[seq_size];
        /* factorisation requires a 64‑bit value */
        uint64_t prev = (uint64_t)prev_val;
        uint16_t exps[32];
        size_t   exps_cnt;
        factor_uint64(prev, exps, &exps_cnt);

        uint64_t best_val = UINT64_MAX;

        /* loop until we locate a usable candidate */
        while (best_val == UINT64_MAX) {
            for (size_t i = 0; i < exps_cnt; ++i) {
                uint16_t e = exps[i];
                if (e >= MAX_EXP) continue;     /* should not happen */

                ExpList *L = &exp_lists[e];

                /* skip already used numbers */
                while (L->pos < L->size && used[L->data[L->pos]])
                    ++L->pos;

                if (L->pos >= L->size) {        /* need more numbers for this exp */
                    size_t new_limit = max_limit * 2 + 1000;
                    if (new_limit <= max_limit) new_limit = max_limit + 1000;
                    extend_lists_up_to(new_limit);
                    best_val = UINT64_MAX;       /* restart search */
                    break;
                } else {
                    uint64_t cand = L->data[L->pos];
                    if (cand < best_val) best_val = cand;
                }
            }
            /* if we had to extend once, best_val is still UINT64_MAX and we try again */
            if (best_val != UINT64_MAX) break;
        }

        /* record the selected term */
        used[best_val] = 1;
        ensure_seq_capacity(seq_size + 1);
        seq[seq_size + 1] = (seq_t)best_val;
        seq_size++;
    }

    return seq[(size_t)n];
}
