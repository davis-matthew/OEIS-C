/*
 * A336676  Irregular triangle read by rows: T(n, k) is the number of
 *          self-conjugate partitions of n into exactly k parts, for n >= 0
 *          and 2*k - 1 <= n <= k^2.
 * https://oeis.org/A336676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   A336676 – irregular triangle read by rows. a(n) is the list of
 *            all T(n,k) with 2*k-1 ≤ n ≤ k², in row‑major order. T(n,0)=0 for
 *            n>0, T(0,0)=1, and T(n,k)=∑_{i<k} T(n-2k+1,i) (n>0)
 * Range:     u8 n <= 2151, u16 n <= 9772, u32 n <= 9772, u64 n <= 9772,
 *            u128 n <= 9772, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simple integer square‑root (floor). */
static unsigned long long isqrt_u64(unsigned long long v)
{
    unsigned long long r = 0, bit = 1ULL << 62;
    while (bit > v) bit >>= 2;
    while (bit) {
        unsigned long long t = r + bit;
        if (t * t <= v) r = t;
        bit >>= 2;
    }
    return r;
}

/*------------------------------------------------------------------*/
/*  Storage for the flattened triangle (a(n)).                       */
/*------------------------------------------------------------------*/
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t *terms = NULL;
static size_t terms_cap = 0;
static size_t terms_len = 0;

/* Reallocate terms array if needed. */
static void ensure_terms_cap(size_t need)
{
    if (terms_cap < need) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 64;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
        terms = tmp;
        terms_cap = new_cap;
    }
}

/* Append a single value to the terms list. */
static void add_term(seq_t v)
{
    ensure_terms_cap(terms_len + 1);
    terms[terms_len++] = v;
}

/*------------------------------------------------------------------*/
/*  Storage for the 2‑D array T[n][k].                              */
/*------------------------------------------------------------------*/
static seq_t **T = NULL;
static size_t *T_sizes = NULL;   /* size of each row (k_max+1) */
static size_t T_cap = 0;
static size_t next_n = 0;        /* next n to process */

/* Ensure rows 0..n are allocated. */
static void ensure_row(size_t n)
{
    while (T_cap <= n) {
        size_t new_cap = T_cap ? T_cap * 2 : 8;
        while (new_cap <= n) new_cap <<= 1;
        seq_t **tmp_T = (seq_t**)realloc(T, new_cap * sizeof(seq_t*));
        size_t *tmp_sz = (size_t*)realloc(T_sizes, new_cap * sizeof(size_t));
        for (size_t i = T_cap; i < new_cap; ++i) {
            tmp_T[i] = NULL;
            tmp_sz[i] = 0;
        }
        T = tmp_T;
        T_sizes = tmp_sz;
        T_cap = new_cap;
    }
}

/*------------------------------------------------------------------*/
/*  Generate terms until index 'target' is filled.                    */
/*------------------------------------------------------------------*/
static void generate_up_to(idx_t target)
{
    while (terms_len <= (size_t)target) {
        ensure_row(next_n);

        /* ----- n == 0 ------------------------------------------------- */
        if (next_n == 0) {
            seq_t val = 1;            /* T(0,0) = 1 */
            add_term(val);
            T[0] = (seq_t*)calloc(1, sizeof(seq_t));
            T_sizes[0] = 1;
            T[0][0] = val;
            ++next_n;
            continue;
        }

        /* ----- bound for k --------------------------------------------- */
        unsigned long long sqrt_n = isqrt_u64(next_n);
        if (sqrt_n * sqrt_n < next_n) ++sqrt_n;                /* ceil(sqrt(n)) */
        size_t k_min = (size_t)sqrt_n;                        /* lowest possible k */
        size_t k_max = (next_n + 1) >> 1;                      /* highest possible k */

        /* Allocate the row for this n. */
        size_t row_size = k_max + 1;                          /* indices 0..k_max */
        T[next_n] = (seq_t*)calloc(row_size, sizeof(seq_t));
        T_sizes[next_n] = row_size;

        /* Base case for k = 0.  T(n,0)=0 for n>0. */
        T[next_n][0] = 0;

        /* Compute all T(next_n, k) for k = 1 .. k_max. */
        for (size_t k = 1; k <= k_max; ++k) {
            /* If n > k² the recurrence yields 0; skip computation. */
            if ((seq_t)next_n > (seq_t)k * (seq_t)k) {
                T[next_n][k] = 0;
                continue;
            }

            size_t sub_n = next_n - 2*k + 1;   /* n - 2k + 1  ≥ 0 from range */
            seq_t sum = 0;
            seq_t *sub_arr = T[sub_n];
            size_t sub_size = T_sizes[sub_n];
            for (size_t i = 0; i < k; ++i) {
                if (i < sub_size) sum += sub_arr[i];
            }
            T[next_n][k] = sum;
        }

        /* Append values that are actually part of the triangle. */
        for (size_t k = k_min; k <= k_max; ++k) {
            if ((seq_t)next_n <= (seq_t)k * (seq_t)k)   /*  n ≤ k²  */
                add_term(T[next_n][k]);
        }

        ++next_n;
    }
}

/*------------------------------------------------------------------*/
/*  Public entry point.                                            */
/*------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    generate_up_to(n);
    return terms[(size_t)n];
}
