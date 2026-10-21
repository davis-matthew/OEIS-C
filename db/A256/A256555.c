/*
 * A256555  Number of ways to write n as the sum of two (unordered) distinct
 *          elements of the set {floor(p/3): p is prime}.
 * https://oeis.org/A256555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2308, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A256555
   Number of ways to write n as the sum of two (unordered) distinct
   elements of the set {⌊p/3⌋ : p is prime}.

   a(n) = |{ (x,y) : x<y , x∈S , y∈S , x+y=n }|
   where S = {⌊p/3⌋ : p prime}.
   This implementation builds the set S for a given n and counts
   unordered distinct pairs with two‑pointer search.
   The algorithm is O(π(3n)) time and O(π(3n)) space.
*/

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* name of the function: seq_t a(idx_t n) */
seq_t a(idx_t n)
{
    /* offset 1 – a(0) unused */
    if (n == 0) return (seq_t)0;

    /* maximum prime needed: p ≤ 3*n + 2  */
    idx_t limit = 3 * n + 2;

    /* sieve of Eratosthenes */
    size_t sz = (size_t)limit + 1;
    unsigned char *is_prime = (unsigned char *)malloc(sz);
    if (!is_prime) return (seq_t)0;
    memset(is_prime, 1, sz);
    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; (i * i) <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* collect distinct ⌊p/3⌋ values (≤ n) into a sorted array */
    size_t cap = 4, cnt = 0;
    idx_t *vals = (idx_t *)malloc(cap * sizeof(idx_t));
    if (!vals) { free(is_prime); return (seq_t)0; }

    idx_t prev = (idx_t)-1;
    for (idx_t p = 2; p <= limit; ++p) {
        if (!is_prime[p]) continue;
        idx_t v = p / 3;
        if (v > n) continue;
        if (v == prev) continue;
        if (cnt == cap) {
            size_t new_cap = cap * 2;
            idx_t *tmp = (idx_t *)realloc(vals, new_cap * sizeof(idx_t));
            if (!tmp) { free(is_prime); free(vals); return (seq_t)0; }
            vals = tmp; cap = new_cap;
        }
        vals[cnt++] = v;
        prev = v;
    }
    free(is_prime);

    /* count unordered distinct pairs summing to n */
    seq_t ways = 0;
    size_t i = 0, j = (cnt != 0 ? cnt - 1 : 0);
    while (i < j) {
        idx_t sum = vals[i] + vals[j];
        if (sum == n) {
            ++ways;
            ++i;
            if (j == 0) break;   /* avoid underflow of unsigned */
            --j;
        } else if (sum < n) {
            ++i;
        } else {
            if (j == 0) break;
            --j;
        }
    }

    free(vals);
    return ways;
}
