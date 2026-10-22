/*
 * A353125  a(1)=2. If a(n) is a novel term, a(n+1) = sopfr(a(n)), else if
 *          there are k occurrences of a(j)=a(n), (1<=j<=n), a(n+1)=k*a(n).
 * https://oeis.org/A353125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2. For n>1: let cur = a(n-1). Let k be the number of
 *            indices i<n with a(i)=cur (including i=n-1 itself). If k==1 (cur
 *            appears for the first time) then a(n)=sopfr(cur) otherwise
 *            a(n)=k*cur. Here sopfr(x) is the sum of the prime factors of x
 *            counted with multiplicity
 * Range:     u8 n <= 96, u16 n <= 1468, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static inline seq_t sopfr_uint64(uint64_t m)
{
    uint64_t sum = 0;
    while ((m & 1ULL) == 0) { sum += 2; m >>= 1; }
    for (uint64_t p = 3; p * p <= m; p += 2)
        while (m % p == 0) { sum += p; m /= p; }
    if (m > 1) sum += m;
    return (seq_t)sum;
}

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *terms = NULL;
    static idx_t terms_len = 0;          /* number of terms stored */

    /* initialise with a(1)=2 if needed */
    if (terms_len == 0) {
        terms_len = 1;
        terms = (seq_t *)realloc(terms, terms_len * sizeof(seq_t));
        if (!terms) abort();
        terms[0] = 2;
    }

    /* extend cache up to n if required */
    if (terms_len < n) {
        idx_t new_len = n;
        seq_t *tmp = (seq_t *)realloc(terms, new_len * sizeof(seq_t));
        if (!tmp) abort();
        terms = tmp;

        /* compute missing terms: indices terms_len .. n-1 (0‑based) */
        for (idx_t i = terms_len; i < n; ++i) {
            seq_t cur = terms[i-1];

            /* count occurrences of cur up to index i-1 */
            idx_t cnt = 0;
            for (idx_t j = 0; j < i; ++j)
                if (terms[j] == cur) ++cnt;

            seq_t next;
            if (cnt == 1) {
                /* novel term: use sopfr */
                uint64_t m = (uint64_t)cur;
                next = sopfr_uint64(m);
            } else {
                /* repeat term: multiply by multiplicity */
                next = cur * cnt;
            }
            terms[i] = next;
        }
        terms_len = new_len;
    }

    return terms[n-1];
}
