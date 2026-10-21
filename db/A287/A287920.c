/*
 * A287920  Triangle T(n,k) read by rows: T(n,k) = floor(prime(n)/prime(k)), n
 *          >= k >= 1.
 * https://oeis.org/A287920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = floor(prime(m)/prime(k)) where m is the unique integer
 *            with m(m‑1)/2 < n ≤ m(m+1)/2 and k = n − m(m‑1)/2. The triangle
 *            T(n,k)=⌊prime(n)/prime(k)⌋ is read by rows
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t primes[200];        /* enough for n up to ~10 000 */
static idx_t primes_count = 0;      /* number of primes stored */
static uint64_t next_candidate = 3; /* next odd number to test */

/* generate primes until we have at least 'needed' primes */
static void ensure_primes(idx_t needed)
{
    if (primes_count >= needed) return;
    if (primes_count == 0) {
        primes[0] = 2;
        primes_count = 1;
    }
    while (primes_count < needed) {
        uint64_t cand = next_candidate;
        bool isprime = true;
        for (idx_t i = 0; i < primes_count; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) { isprime = false; break; }
        }
        if (isprime) primes[primes_count++] = cand;
        next_candidate += 2;
    }
}

seq_t a(idx_t n)
{
    /* find row m such that m(m‑1)/2 < n ≤ m(m+1)/2 */
    idx_t m = 1;
    while ((m * (m + 1)) / 2 < n) ++m;
    idx_t prev_sum = (m - 1) * m / 2;
    idx_t k = n - prev_sum;          /* column index (1‑based) */

    ensure_primes(m);                 /* need primes up to m */

    uint64_t pn = primes[m - 1];      /* prime(m)   */
    uint64_t pk = primes[k - 1];      /* prime(k)   */
    uint64_t q  = pn / pk;            /* integer division */

    return (seq_t)q;
}
