/*
 * A363489  Rounded mean of the multiset of prime indices of n.
 * https://oeis.org/A363489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round_half_even( (∑ e * π(p)) / (∑ e) ), with a(1)=0
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   _smallest_prime_factor, compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here π(p) is the index of prime p (the number of primes ≤ p). The rounding
   rule is "round half to even": if the fractional part is exactly 0.5,
   the result rounds to the nearest even integer. */

#include "../utils/_smallest_prime_factor.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n == 1) return 0;

    uint64_t v = (uint64_t)n;
    seq_t sum = 0;
    seq_t cnt = 0;

    while (v > 1) {
        uint64_t p = _smallest_prime_factor(v);
        if (p == 0) break;                /* safety, should not happen */
        uint64_t e = 0;
        while (v % p == 0) {
            v /= p;
            ++e;
        }
        idx_t pi = (idx_t)compute_pi(p);   /* index of prime p, 1‑based */
        sum += (seq_t)pi * e;
        cnt += e;
    }

    if (cnt == 0) return 0; /* only possible if n==1 */

    seq_t q = sum / cnt;          /* floor of the mean */
    seq_t r = sum % cnt;          /* remainder */
    seq_t twice = r * 2;

    if (twice < cnt)          return q;          /* fraction < 0.5 */
    if (twice > cnt)          return q + 1;      /* fraction > 0.5 */

    /* fraction == 0.5: round to nearest even integer */
    return (q & 1) == 0 ? q : q + 1;
}
