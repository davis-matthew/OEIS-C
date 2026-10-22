/*
 * A373873  a(n) = Sum_{k=1..n} k! * k^(n-2) * Stirling2(n,k).
 * https://oeis.org/A373873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A373873: a(n) = Sum_{k=1..n} k! * k^(n-2) * Stirling2(n,k) */
/* Offset: 0 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

static bool initialized = false;
static idx_t          max_n          = 0;    /* largest n computed so far   */
static size_t         a_cap          = 0;    /* capacity of a array          */
static seq_t          *a_vals         = NULL; /* a[0..max_n]                  */
static size_t         fact_cap       = 0;    /* capacity of factorial array   */
static seq_t          *fact          = NULL; /* factorial[0..max_n]           */
static size_t         S_cap          = 0;    /* capacity of Stirling array    */
static seq_t          *S             = NULL; /* S[k] holds S(current_n,k)    */
static size_t         pow_cap        = 0;    /* capacity of power array      */
static seq_t          *pow_arr       = NULL; /* pow_arr[k] = k^(current_n-2)* */

/* Ensure a correctly sized allocation */
static void ensure_cap(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 1 : (*cap * 2);
    while (new_cap < needed) new_cap *= 2;
    void *newp = realloc(*ptr, new_cap * elem_size);
    *ptr = newp;
    *cap = new_cap;
}

/* Initialise static arrays for n = 0 */
static void initialise(void)
{
    if (initialized) return;
    initialized = true;

    /* a_vals[0] = 0 */
    a_cap  = 4;
    a_vals = (seq_t*)calloc(a_cap, sizeof(seq_t));
    a_vals[0] = 0;

    /* factorial[0] = 1 */
    fact_cap = 4;
    fact = (seq_t*)calloc(fact_cap, sizeof(seq_t));
    fact[0] = 1;

    /* S[0] = 1 (for Stirling(0,0)=1) */
    S_cap = 4;
    S = (seq_t*)calloc(S_cap, sizeof(seq_t));
    S[0] = 1;

    /* pow_arr not needed for n=0; allocate empty */
    pow_cap = 4;
    pow_arr = (seq_t*)calloc(pow_cap, sizeof(seq_t));
}

/* Main sequence function */
seq_t a(idx_t n)
{
    initialise();

    if (n <= max_n)
        return a_vals[n];

    for (idx_t cur = max_n + 1; cur <= n; ++cur)
    {
        /* extend factorial */
        ensure_cap((size_t)cur + 1, &fact_cap, (void **)&fact, sizeof(seq_t));
        fact[cur] = fact[cur - 1] * (seq_t)cur;

        /* extend Stirling array */
        ensure_cap((size_t)cur + 1, &S_cap, (void **)&S, sizeof(seq_t));
        for (idx_t k = cur; k >= 1; --k)
        {
            seq_t tmp = (seq_t)k * S[k] + S[k - 1];
            S[k] = tmp;
        }
        S[0] = 0;

        /* extend power array */
        ensure_cap((size_t)cur + 1, &pow_cap, (void **)&pow_arr, sizeof(seq_t));
        /* update powers for existing k < cur */
        for (idx_t k = 1; k < cur; ++k)
            pow_arr[k] = pow_arr[k] * (seq_t)k;
        /* compute power for new k = cur */
        seq_t pow_new;
        if (cur <= 2)
            pow_new = 1; /* 1^(-1)=1, 2^0=1 */
        else
            pow_new = big_pow((seq_t)cur, (seq_t)(cur - 2));
        pow_arr[cur] = pow_new;

        /* compute a[cur] */
        seq_t sum = 0;
        for (idx_t k = 1; k <= cur; ++k)
        {
            seq_t term = fact[k] * pow_arr[k] * S[k];
            sum += term;
        }
        ensure_cap((size_t)cur + 1, &a_cap, (void **)&a_vals, sizeof(seq_t));
        a_vals[cur] = sum;
    }

    max_n = n;
    return a_vals[n];
}
