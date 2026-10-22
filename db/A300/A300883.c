/*
 * A300883  Number of nX3 0..1 arrays with every element equal to 0, 1, 3, 4
 *          or 5 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A300883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The sequence satisfies the empirical 21‑step linear recurrence
 *            a(n) = 5 a(n‑2) + 7 a(n‑3) + 6 a(n‑4) −15 a(n‑5) −47 a(n‑6) −47
 *            a(n‑7) 5 a(n‑8) +102 a(n‑9) +86 a(n‑10) +23 a(n‑12) +28 a(n‑13)
 *            −51 a(n‑14) −119 a(n‑15) −24 a(n‑16) +111 a(n‑17) +69 a(n‑18)
 *            −21 a(n‑19) −27 a(n‑20) − 6 a(n‑21) for n >...
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 49, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static seq_t *seq = NULL;          /* 1‑indexed cache of computed terms   */
static size_t cache_cap = 0;       /* current allocated size (>=n+1)      */
static size_t cache_max = 0;       /* largest n with a computed value    */

/* Ensure that seq has space for indices up to need-1 (i.e. size >= need) */
static void ensure_cache(size_t need)
{
    if (cache_cap < need)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 32;
        if (new_cap < need) new_cap = need;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!seq) exit(EXIT_FAILURE); /* allocation failed */
        /* zero the new part – not strictly needed but harmless */
        for (size_t i = cache_cap; i < new_cap; ++i) seq[i] = 0;
        cache_cap = new_cap;
    }
}

/* Initialise the first 25 terms; 25th term is known from OEIS. */
static void init_sequence(void)
{
    if (cache_max) return;          /* already initialised */

    ensure_cache(26);               /* indices 0..25, we use 1..25 */

    seq[ 1] = 3;    seq[ 2] = 2;    seq[ 3] = 4;
    seq[ 4] = 8;    seq[ 5] = 17;   seq[ 6] = 57;
    seq[ 7] = 148;  seq[ 8] = 356;  seq[ 9] = 864;
    seq[10] = 2272; seq[11] = 5766; seq[12] = 14438;
    seq[13] = 36379; seq[14] = 92632; seq[15] = 234473;
    seq[16] = 592300; seq[17] = 1497878; seq[18] = 3794198;
    seq[19] = 9602820; seq[20] = 24292985; seq[21] = 61469034;
    seq[22] = 155580439; seq[23] = 393724501; seq[24] = 996303601;
    seq[25] = 2521217660;

    cache_max = 25;
}

/* Compute the next term (n > 25) using the recurrence. */
static seq_t compute_next(idx_t n)
{
    /* coefficients are all integers; cast to seq_t to preserve type. */
    return
        (seq_t)5   * seq[n-2] + (seq_t)7   * seq[n-3] + (seq_t)6   * seq[n-4]
      - (seq_t)15  * seq[n-5]          - (seq_t)47  * seq[n-6] - (seq_t)47  * seq[n-7]
      + (seq_t)5   * seq[n-8]          + (seq_t)102 * seq[n-9] + (seq_t)86  * seq[n-10]
      + (seq_t)23  * seq[n-12]         + (seq_t)28  * seq[n-13]
      - (seq_t)51  * seq[n-14]         - (seq_t)119 * seq[n-15] - (seq_t)24  * seq[n-16]
      + (seq_t)111 * seq[n-17]         + (seq_t)69  * seq[n-18]
      - (seq_t)21  * seq[n-19]         - (seq_t)27  * seq[n-20] - (seq_t)6   * seq[n-21];
}

/* The required function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n=0, but safety */

    ensure_cache((size_t)n + 1);
    init_sequence();                     /* guarantees seq[1..25] exist */

    if (cache_max < (size_t)n)           /* extend cache up to n */
    {
        for (size_t i = cache_max + 1; i <= (size_t)n; ++i)
            seq[i] = compute_next((idx_t)i);
        cache_max = (size_t)n;
    }
    return seq[n];
}
