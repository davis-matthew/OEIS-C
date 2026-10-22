/*
 * A354085  Least prime q > p = prime(n) such that Omega(p+q) = 3.
 * https://oeis.org/A354085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime q > prime(n) such that Omega(prime(n)+q)=3
 * Range:     u8 n <= 48, u16 n <= 6530, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, get_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/next_prime.h"
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* offset behaviour is irrelevant */
    uint64_t p = get_prime(n);            /* nth prime, 1‑indexed */
    for (uint64_t q = next_prime(p); ; q = next_prime(q))
    {
        idx_t sum = p + q;                /* p+q fits into uint64_t for practical n */
        if (bigomega(sum) == (seq_t)3)    /* Omega(p+q) == 3 */
            return (seq_t)q;
    }
}
