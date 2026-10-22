/*
 * A333339  a(n) is the smallest positive number k such that n divides 3^k -
 *          k.
 * https://oeis.org/A333339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that n divides 3^k - k
 * Range:     u8 n <= 121, u16 n <= 9931, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;      /* memoisation array, index 0 unused */
static idx_t  a_cache_len = 0;

/* Ensure the cache can hold indices up to 'max_index'. Empty entries are 0. */
static void ensure_cache(idx_t max_index) {
    if (max_index < a_cache_len)
        return;
    idx_t newlen = a_cache_len == 0 ? 1 : a_cache_len;
    while (newlen <= max_index) newlen <<= 1;   /* grow exponentially */
    a_cache = (seq_t *)realloc(a_cache, newlen * sizeof(seq_t));
    for (idx_t i = a_cache_len; i < newlen; ++i)
        a_cache[i] = 0;
    a_cache_len = newlen;
}

seq_t a(idx_t n) {
    /* The problem guarantees that a(n) >= 1 for all n >= 1.  */
    if (n == 0) return 0;                       /* undefined but safe path */
    ensure_cache(n);
    if (a_cache[n] != 0) return a_cache[n];     /* already computed   */

    idx_t pow_mod = 3 % n;         /* 3^1 mod n */
    for (idx_t k = 1; ; ++k) {     /* search k >= 1  */
        if (pow_mod == (k % n)) {  /* 3^k ≡ k (mod n)  */
            a_cache[n] = (seq_t)k;
            return a_cache[n];
        }
        pow_mod = (pow_mod * 3) % n;     /* next power of 3 modulo n */
    }
}
