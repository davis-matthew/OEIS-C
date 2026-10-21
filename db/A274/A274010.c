/*
 * A274010  Boris Stechkin function: a(n) is the number of m with 2 <= m <= n
 *          and floor(n(m-1)/m) divisible by m-1.
 * https://oeis.org/A274010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(n) + tau(n-1) - 2 (for n>=2) where tau(k) is the
 *            number of divisors of k. a(0) = a(1) = 0. The implementation
 *            uses a precomputed smallest‑prime‑factor table to obtain tau in
 *            O(n log log n) time and uses only integer arithmetic. It
 *            memoises the tables so that calls are O(1) after the fi...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>   /* for uint8_t */

/* the types seq_t and idx_t are defined by the build environment */
extern unsigned long long __atomic_load_n (void const *, int);  /* dummy to avoid unused */

static idx_t   max_n   = 0;          /* highest index for which tables are computed */
static idx_t *spf     = NULL;        /* smallest prime factors, indices 0..max_n */
static uint8_t *exp_arr  = NULL;     /* exponent of spf[i] in i, indices 0..max_n */
static seq_t *tau_arr   = NULL;      /* divisor counts tau[i], indices 0..max_n */

/* recompute tables for all indices up to limit (inclusive) */
static void recompute(idx_t limit)
{
    idx_t i, j;
    /* allocate/reallocate memory */
    spf    = (idx_t*)realloc(spf,    (limit+1)*sizeof(idx_t));
    exp_arr= (uint8_t*)realloc(exp_arr,(limit+1)*sizeof(uint8_t));
    tau_arr= (seq_t*)realloc(tau_arr,(limit+1)*sizeof(seq_t));
    if (!spf || !exp_arr || !tau_arr) abort();

    /* initialise */
    spf[0]   = 0;
    spf[1]   = 1;
    exp_arr[0] = 0;
    exp_arr[1] = 0;
    tau_arr[0] = 0;
    tau_arr[1] = 1;

    /* compute smallest prime factors via a simple sieve */
    for (i = 2; i <= limit; ++i)
        spf[i] = 0;                   /* mark as unassigned */

    for (i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {            /* i is prime */
            spf[i] = i;
            if (i <= limit / i) {     /* avoid overflow in i*i */
                for (j = i * i; j <= limit; j += i)
                    if (spf[j] == 0)
                        spf[j] = i;
            }
        }
    }

    /* compute exponent of spf[i] and divisor count tau[i] */
    for (i = 2; i <= limit; ++i) {
        idx_t p = spf[i];
        idx_t m = i / p;
        if (spf[m] == p)          /* same prime as in m */
            exp_arr[i] = exp_arr[m] + 1;
        else
            exp_arr[i] = 1;

        seq_t tau_m = tau_arr[m];
        tau_arr[i] = tau_m * (seq_t)(exp_arr[i] + 1) /
                       (seq_t)(exp_arr[i]);
    }

    max_n = limit;
}

/* ensure tables cover at least up to n */
static void ensure(idx_t n)
{
    if (n <= max_n) return;
    recompute(n);
}

/* -------------------------------------------------------------- */
/*  The required sequence function                                   */
/* -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return 0;

    ensure(n);
    seq_t res = tau_arr[n] + tau_arr[n-1] - 2;
    return res;
}
