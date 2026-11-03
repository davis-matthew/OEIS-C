/*
 * A333752  Sum of squarefree divisors of n that are <= sqrt(n).
 * https://oeis.org/A333752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 90 terms through n = 90 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A333752: sum of squarefree divisors of n that are <= sqrt(n) */
#include <stdint.h>
#include <stdbool.h>

/* Recursively sum the square‑free products not exceeding the limit */
static seq_t sum_squarefree_products(uint64_t *primes, int pos, int count, seq_t prod, idx_t limit)
{
    seq_t total = 0;
    for (int i = pos; i < count; ++i) {
        seq_t new_prod = prod * (seq_t)primes[i];
        /* Skip if the product would overflow the (integer) limit */
        if (new_prod > (seq_t)limit) continue;
        total += new_prod;
        total += sum_squarefree_products(primes, i + 1, count, new_prod, limit);
    }
    return total;
}

/* Integer square‑root: floor(sqrt(n)) */
static idx_t int_sqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        if (mid <= n / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, but harmless */

    idx_t limit = int_sqrt_u64(n);

    /* Factorise n and store unique prime factors */
    uint64_t primes[10];
    int prime_count = 0;
    uint64_t temp = n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            primes[prime_count++] = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) primes[prime_count++] = temp;

    /* Sum all square‑free divisors <= limit */
    seq_t sum = 1;  /* the divisor 1 is always included */
    sum += sum_squarefree_products(primes, 0, prime_count, 1, limit);

    return sum;
}
