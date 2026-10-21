/*
 * A286223  Number of partitions of n into distinct parts with an even number
 *          of prime divisors (counted with multiplicity).
 * https://oeis.org/A286223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts with an
 *            even number of prime divisors (counted with multiplicity)
 * Range:     u8 n <= 73, u16 n <= 184, u32 n <= 534, u64 n <= 1717,
 *            u128 n <= 5973, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;           /* dp[sum] = number of valid partitions of sum   */
static size_t dp_cap = 0;          /* current capacity of dp                        */
static idx_t *spf = NULL;          /* smallest prime factor table                  */
static size_t spf_cap = 0;         /* current capacity of spf                      */
static unsigned char *parity = NULL;/* parity[k] = 1 if Ω(k) even, 0 if odd          */
static size_t parity_cap = 0;      /* current capacity of parity                   */
static idx_t max_n_processed = 0;  /* largest n for which dp has been computed      */

/* Resize an array of some type when needed. */
#define RESIZE_ARRAY(ptr, current_cap, new_cap, type)             \
    do {                                                         \
        if ((current_cap) < (new_cap)) {                         \
            type *tmp = (type *)realloc((ptr), (new_cap) * sizeof(type)); \
            if (!tmp) abort();                                   \
            (ptr) = tmp;                                         \
            (current_cap) = (new_cap);                           \
        }                                                        \
    } while (0)

seq_t a(idx_t n)
{
    /* Handle the trivial case a(0) = 1 */
    if (n == 0) {
        if (dp_cap < 1) {
            dp = (seq_t *)realloc(dp, sizeof(seq_t));
            if (!dp) abort();
            dp_cap = 1;
        }
        dp[0] = (seq_t)1;
        max_n_processed = 0;
        return dp[0];
    }

    /* If we already computed up to a larger n, simply return the cached value */
    if (n <= max_n_processed) {
        return dp[(size_t)n];
    }

    /* Ensure the arrays are large enough for the current n */
    RESIZE_ARRAY(dp, dp_cap, (size_t)n + 1, seq_t);
    RESIZE_ARRAY(spf, spf_cap, (size_t)n + 1, idx_t);
    RESIZE_ARRAY(parity, parity_cap, (size_t)n + 1, unsigned char);

    /* Compute smallest prime factor (spf) table up to n */
    spf[0] = 0;
    spf[1] = 1;
    for (size_t i = 2; i <= (size_t)n; ++i) {
        spf[i] = 0;
    }
    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {            /* i is prime */
            spf[i] = i;
            if (i <= n / i) {
                for (idx_t j = i * i; j <= n; j += i) {
                    if (spf[j] == 0) {
                        spf[j] = i;
                    }
                }
            }
        }
    }

    /* Compute parity of Ω(k) for all k <= n */
    parity[1] = 1;                        /* Ω(1)=0 is even */
    for (idx_t k = 2; k <= n; ++k) {
        if (spf[k] == k) {                /* k is prime: Ω(k)=1 odd */
            parity[k] = 0;
        } else {
            parity[k] = parity[k / spf[k]] ^ 1;  /* flip parity of Ω(k/spf[k]) */
        }
    }

    /* Initialize dp array */
    dp[0] = (seq_t)1;
    for (size_t i = 1; i <= (size_t)n; ++i) {
        dp[i] = (seq_t)0;
    }

    /* Dynamic programming over distinct parts with even Ω */
    for (idx_t part = 1; part <= n; ++part) {
        if (!parity[part]) {  /* skip parts with odd Ω */
            continue;
        }
        for (idx_t sum = n; sum >= part; --sum) {
            dp[(size_t)sum] += dp[(size_t)(sum - part)];
        }
    }

    /* Record that we have processed up to this n */
    max_n_processed = n;
    return dp[(size_t)n];
}
