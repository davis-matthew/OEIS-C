/*
 * A247254  a(1)=0; for n>1, a(n+1)= a(n)- A004648(n) if a(n)>= A004648(n)
 *          else a(n) + A004648(n).
 * https://oeis.org/A247254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; a(n+1) = a(n) - b(n) if a(n) >= b(n) else a(n) + b(n),
 *            where b(n) = A004648(n) = prime(n) mod n
 * Range:     u8 n <= 159, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9057 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;
    /* need primes 1..n-1 ; upper bound for prime(n) */
    double dn = (double)n + 1.0;
    idx_t limit = 100;
    if (n >= 6) limit = (idx_t)(dn * (log(dn) + log(log(dn)))) + 10;
    bool *s = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    memset(s, 1, (size_t)(limit + 1) * sizeof(bool));
    s[0] = 0; s[1] = 0;
    for (idx_t i = 2; i * i <= limit; i++)
        if (s[i])
            for (idx_t j = i * i; j <= limit; j += i) s[j] = 0;
    seq_t v = 0;
    idx_t k = 0;
    for (idx_t p = 2; p <= limit && k < n - 1; p++) {
        if (!s[p]) continue;
        k++;
        seq_t b = (seq_t)(p % k);
        if (v >= b) v = v - b;
        else v = v + b;
    }
    free(s);
    return v;
}
