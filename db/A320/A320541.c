/*
 * A320541  Triangle read by rows: T(n,k) (1<=k<=n) = Sum_{i=1..n, j=1..k,
 *          gcd(i,j)=1} (n+1-i)*(k+1-j).
 * https://oeis.org/A320541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   [T(n,k) = Sum_{i=1..n, j=1..k, gcd(i,j)=1} (n+1-i)*(k+1-j)]
 *            Triangle read by rows: a(n) = T(r,k) where r is the row
 *            containing the nth term and k is its column index
 * Range:     u8 n <= 20, u16 n <= 324, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function works for any seq_t and idx_t types defined by the build. */
#include <stdint.h>
#include <stddef.h>

/* Return T(n,k) as defined above. */
static seq_t compute_T(idx_t n, idx_t k)
{
    seq_t sum = 0;
    seq_t base = (seq_t)k * (seq_t)(k + 1) / 2;     /* sum_{j=1..k} (k+1-j) */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t weight_i = (seq_t)(n + 1 - i);
        seq_t coprime_sum = base;

        /* Find distinct prime factors of i by trial division. */
        idx_t temp = i;
        idx_t primes[10];
        size_t pcount = 0;
        for (idx_t p = 2; p * p <= temp; ++p) {
            if (temp % p == 0) {
                primes[pcount++] = p;
                while (temp % p == 0) temp /= p;
            }
        }
        if (temp > 1) primes[pcount++] = temp;

        if (pcount > 0) {
            int subsets = 1 << (int)pcount;
            for (int mask = 1; mask < subsets; ++mask) {
                idx_t d = 1;
                int bits = 0;
                for (int b = 0; b < (int)pcount; ++b) {
                    if (mask & (1 << b)) {
                        d *= primes[b];
                        ++bits;
                    }
                }
                idx_t M = k / d;                      /* count of multiples */
                seq_t Mseq = (seq_t)M;
                seq_t part = Mseq * (seq_t)(k + 1) -
                             (seq_t)d * Mseq * (seq_t)(M + 1) / 2;
                if (bits & 1)
                    coprime_sum -= part;
                else
                    coprime_sum += part;
            }
        }

        sum += weight_i * coprime_sum;
    }
    return sum;
}

/* Main entry: a(n) for the OEIS triangular sequence. */
seq_t a(idx_t n)
{
    /* Determine row r such that r*(r+1)/2 >= n, with k = n - (r-1)*r/2. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;
    idx_t prev = ((r - 1) * r) / 2;
    idx_t k = n - prev;
    return compute_T(r, k);
}
