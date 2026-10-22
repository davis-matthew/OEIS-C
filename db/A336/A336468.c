/*
 * A336468  a(n) = A336466(phi(n)), where A336466 is fully multiplicative with
 *          a(p) = A000265(p-1) for prime p, with A000265(k) giving the odd
 *          part of k.
 * https://oeis.org/A336468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = A336466(phi(n)) where A336466 is multiplicative with a(p)
 *            = odd_part(p-1)
 * Range:     u8 n <= 869, u16 n <= 8957, u32 n <= 8957, u64 n <= 8957,
 *            u128 n <= 8957, big any n
 * Helpers:   compute_phi_seq, odd_part_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/compute_phi_seq.h"
#include "../utils/odd_part_seq.h"

/* Factorisation of a 64‑bit integer n.
   pr[i] = prime factor, ex[i] = its exponent.  At most 32 distinct primes
   are needed for any n < 10^4, which is sufficient for the required limits. */
static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            uint64_t e = 0;
            while (n % p == 0)
            {
                n /= p;
                ++e;
            }
            pr[c] = p;
            ex[c] = e;
            ++c;
        }
    }
    if (n > 1)
    {
        pr[c] = n;
        ex[c] = 1;
        ++c;
    }
    *cnt = c;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* unused, but harmless */

    uint64_t n64 = n;

    /* Factor n to compute phi(n). */
    uint64_t pr_n[32], ex_n[32];
    size_t cnt_n;
    factor_uint64(n64, pr_n, ex_n, &cnt_n);

    seq_t phi_n = compute_phi_seq(n64, pr_n, ex_n, cnt_n);

    /* Factor phi(n) and apply the multiplicative rule of A336466. */
    uint64_t phi64 = (uint64_t)phi_n;    /* phi_n fits in 64‑bit */
    uint64_t pr_phi[32], ex_phi[32];
    size_t cnt_phi;
    factor_uint64(phi64, pr_phi, ex_phi, &cnt_phi);

    seq_t result = 1;
    for (size_t i = 0; i < cnt_phi; ++i)
    {
        seq_t odd = odd_part_seq(pr_phi[i] - 1);
        seq_t pow_val = 1;
        for (uint64_t e = 0; e < ex_phi[i]; ++e)
            pow_val *= odd;
        result *= pow_val;
    }
    return result;
}
