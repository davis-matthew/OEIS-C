/*
 * A400150  a(n) = |2*a(n-1) - a(n-2) - 2|, with a(0) = 0 and a(1) = 1.
 * https://oeis.org/A400150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 254, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A400150 – Sequence: a(n) = |2*a(n-1) – a(n-2) – 2|,
   a(0) = 0, a(1) = 1.
   The recurrence uses only integer arithmetic on the supplied types. */

#include <stdlib.h>   /* realloc, NULL */

seq_t a(idx_t n)
{
    /* static cache for arbitrary‑order calls */
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;
    static idx_t  last  = 0;   /* highest index already computed */

    /* initialise cache on the first call */
    if (cache == NULL) {
        /* allocate space for indices 0 and 1 */
        cap   = 2;
        last  = 1;
        cache = (seq_t *)realloc(NULL, sizeof(seq_t) * cap);
        if (cache == NULL) return (seq_t)0;   /* allocation failed */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
    }

    /* trivial base cases */
    if (n == 0) return cache[0];
    if (n == 1) return cache[1];

    /* grow cache if needed (double strategy) */
    if (n >= cap) {
        idx_t new_cap = cap;
        if (new_cap == 0) new_cap = 1;
        while (n >= new_cap) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) return (seq_t)0;   /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* if the value is already computed, return it immediately */
    if (n <= last) return cache[n];

    /* iterate forward from last + 1 up to n, computing the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* compute 2*a(i-1) */
        seq_t two_minus_a = cache[i-1] + cache[i-1];
        /* compute a(i-2) + 2 */
        seq_t a_minus2_plus = cache[i-2] + (seq_t)2;

        /* absolute value of (two_minus_a - a_minus2_plus) */
        if (two_minus_a >= a_minus2_plus) {
            cache[i] = two_minus_a - a_minus2_plus;
        } else {
            cache[i] = a_minus2_plus - two_minus_a;
        }
    }

    last = n;
    return cache[n];
}
