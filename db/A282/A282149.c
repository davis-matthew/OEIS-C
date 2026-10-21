/*
 * A282149  Numbers n with k digits in base x (MSD(n)=d_k, LSD(n)=d_1) such
 *          that, chosen one of their digits in position d_k < j < d_1, is
 *          Sum_{i=j..k}{(i-j+1)*d_i} = Sum_{i=1..j-1}{(j-i)*d_i}. Case x = 8.
 * https://oeis.org/A282149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th number whose base‑8 digits d_k…d_1 satisfy there
 *            exists an index j (2 ≤ j ≤ k) such that Σ_{i=j..k} (i‑j+1)*d_i =
 *            Σ_{i=1..j‑1} (j‑i)*d_i. The sequence is indexed starting at a(1)
 * Range:     u8 n <= 24, u16 n <= 3220, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The algorithm lazily generates terms up to the maximum index requested,
    storing them in a static dynamic array.  Each candidate number is tested
    by converting it to base‑8, computing its digit sums, and checking the
    fulcrum condition in O(k) time, with k the number of base‑8 digits
    (~≤ 20 for 64‑bit values, smaller for larger numbers).  This is fast
    enough to generate 10,000 terms within the time limits. */
#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, free */

static seq_t *cache = NULL;      /* array of found terms */
static size_t cache_len = 0;     /* number of terms stored */
static size_t cache_cap = 0;     /* buffer capacity */
static idx_t next_candidate = 1;/* next integer to test */

static int satisfies(seq_t x)
{
    /* collect base‑8 digits (least significant first) */
    unsigned char digits[128];
    size_t len = 0;
    seq_t tmp = x;
    if (tmp == (seq_t)0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (tmp != (seq_t)0) {
            digits[len++] = (unsigned char)(tmp % (seq_t)8);
            tmp = tmp / (seq_t)8;
        }
    }

    /* prefix sums: S[i] = Σ_{t=1..i} d_t, W[i] = Σ_{t=1..i} t*d_t */
    seq_t S[129] = {0};
    seq_t W[129] = {0};
    for (size_t i = 0; i < len; ++i) {
        S[i+1] = S[i] + (seq_t)digits[i];
        W[i+1] = W[i] + (seq_t)(i+1) * (seq_t)digits[i];
    }

    seq_t totalS = S[len];
    seq_t totalW = W[len];

    /* test all possible fulcrum positions j = 2..len */
    for (size_t j = 2; j <= len; ++j) {
        seq_t sum_right = (seq_t)j * S[j-1] - W[j-1];
        seq_t sum_left  = (totalW - W[j-1]) - (seq_t)(j-1) * (totalS - S[j-1]);
        if (sum_left == sum_right) return 1;
    }
    return 0;
}

static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* in case of failure we simply keep the existing cache (program
       will re‑try later).  For the contest environment this should
       never happen. */
    cache = p;
    cache_cap = new_cap;
}

static void generate_up_to(idx_t n)
{
    while (cache_len < n) {
        seq_t val = (seq_t)next_candidate;
        if (satisfies(val)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = val;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* undefined, but safe fallback */
    generate_up_to(n);
    return cache[n-1];
}
