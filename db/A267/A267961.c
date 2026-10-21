/*
 * A267961  Number of n X 2 0..2 arrays with every repeated value in every row
 *          and column greater than the previous repeated value.
 * https://oeis.org/A267961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for sequence A267961:
   a(n) = 21*a(n-1) -153*a(n-2) +263*a(n-3) +2094*a(n-4) -9792*a(n-5) -2752*a(n-6)
          +86256*a(n-7) -85536*a(n-8) -338048*a(n-9) +554496*a(n-10) +571392*a(n-11)
          -1335296*a(n-12) -98304*a(n-13) +1179648*a(n-14) -524288*a(n-15)
   initial values   a(1) = 9,
                    a(2) = 81,
                    a(3) = 576,
                    a(4) = 3969,
                    a(5) = 25281,
                    a(6) = 155236,
                    a(7) = 915849,
                    a(8) = 5253264,
                    a(9) = 29365561,
                    a(10)= 160731684,
                    a(11)= 863478225,
                    a(12)= 4564353600,
                    a(13)= 23782266225,
                    a(14)= 122339052900,
                    a(15)= 622112365081 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Initial 15 terms of the sequence. */
static const seq_t init15[15] = {
    (seq_t)9,        /* a(1)  */
    (seq_t)81,       /* a(2)  */
    (seq_t)576,      /* a(3)  */
    (seq_t)3969,     /* a(4)  */
    (seq_t)25281,    /* a(5)  */
    (seq_t)155236,   /* a(6)  */
    (seq_t)915849,   /* a(7)  */
    (seq_t)5253264,  /* a(8)  */
    (seq_t)29365561, /* a(9) */
    (seq_t)160731684,/* a(10)*/
    (seq_t)863478225,/* a(11)*/
    (seq_t)4564353600,/* a(12)*/
    (seq_t)23782266225,/* a(13)*/
    (seq_t)122339052900,/* a(14)*/
    (seq_t)622112365081 /* a(15)*/
};

/* Static cache for memoization.  Index 1 corresponds to a(1). */
static seq_t *cache = NULL;
static size_t cache_len = 0; /* maximum number currently cached. */
static size_t cache_cap = 0;

/* Ensure that the cache has capacity for at least 'needed' indices. */
static void ensure_capacity(size_t needed)
{
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) abort();           /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Main function returning a(n).  Non‑static so external linkage. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset but safe */

    size_t target = (size_t)n;            /* requires n <= SIZE_MAX */

    ensure_capacity(target + 1);           /* store up to index 'n' */

    /* Initialise base cases if not yet done. */
    if (cache_len < 15) {
        for (size_t i = cache_len + 1; i <= 15; ++i)
            cache[i] = init15[i - 1];
        cache_len = 15;
    }

    /* Compute missing terms up to 'n' using the recurrence. */
    if (cache_len < target) {
        for (size_t i = cache_len + 1; i <= target; ++i) {
            seq_t n1  = cache[i - 1]  * (seq_t)21;
            seq_t n2  = cache[i - 2]  * (seq_t)153;
            seq_t n3  = cache[i - 3]  * (seq_t)263;
            seq_t n4  = cache[i - 4]  * (seq_t)2094;
            seq_t n5  = cache[i - 5]  * (seq_t)9792;
            seq_t n6  = cache[i - 6]  * (seq_t)2752;
            seq_t n7  = cache[i - 7]  * (seq_t)86256;
            seq_t n8  = cache[i - 8]  * (seq_t)85536;
            seq_t n9  = cache[i - 9]  * (seq_t)338048;
            seq_t n10 = cache[i -10]  * (seq_t)554496;
            seq_t n11 = cache[i -11]  * (seq_t)571392;
            seq_t n12 = cache[i -12]  * (seq_t)1335296;
            seq_t n13 = cache[i -13]  * (seq_t)98304;
            seq_t n14 = cache[i -14]  * (seq_t)1179648;
            seq_t n15 = cache[i -15]  * (seq_t)524288;

            seq_t next = n1 - n2 + n3 + n4 - n5 - n6 + n7 - n8 - n9
                         + n10 + n11 - n12 - n13 + n14 - n15;
            cache[i] = next;
        }
        cache_len = target;
    }

    return cache[target];
}
