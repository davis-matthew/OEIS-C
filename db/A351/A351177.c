/*
 * A351177  Number of distinct residues of k^(n^2) (mod n^2+1), k=0..n^2.
 * https://oeis.org/A351177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
/// a(n) = number of distinct residues of k^(n^2) (mod n^2+1), k=0..n^2
///
/// For each prime power p^a || (n^2+1) let
///   φ = Euler's totient of p^a = p^a - p^(a-1) (p odd) or 2^(a-2) (p=2, a>=2).
///   e = n^2
///   The non-zero residues are the image of the map x↦x^e on the unit group
///   U(p^a).  In a cyclic group the image size is φ/gcd(e,φ); for p=2
///   (a≥3) the group is C2×C_{2^(a-2)} and the image size equals
///   φ / ((e even?2:1)*gcd(e,2^(a-2))).  The factor contributing to the
///   total count is 1 (for the residue 0) plus that image size.
///
///  The total number of distinct residues modulo n^2+1 is the product
///  of the contributions from each prime power factor of n^2+1.
///  For n odd  (so n^2+1 is even but not divisible by 4) the contribution of 2
///  is always 2.  For all other primes the contribution can be computed
///  with 64‑bit integers for the problem sizes considered.
///
*/

#include <stdint.h>

/* Euclidean gcd for 64‑bit unsigned integers. */
static uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Power of a 64‑bit unsigned integer: base^exp  (exp fits in uint64_t). */
static uint64_t pow_u64(uint64_t base, unsigned exp)
{
    uint64_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Compute m = n^2 + 1.  n is small enough that 64‑bit arithmetic suffices. */
    uint64_t nn = (uint64_t)n;
    uint64_t e  = nn * nn;          /* exponent n^2 */
    uint64_t m  = nn * nn + 1ULL;   /* modulus */
    uint64_t tmp = m;

    uint64_t result64 = 1;          /* will accumulate the product of contributions */

    /* Factor 2 separately (only a=1 occurs for n up to 10^4). */
    unsigned exp2 = 0;
    while ((tmp & 1ULL) == 0ULL) {
        tmp >>= 1;
        ++exp2;
    }
    if (exp2) {
        /* For p = 2, exp2 is always 1 for the data range; the contribution is 2. */
        result64 *= 2ULL;
    }

    /* Odd prime factors. */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p != 0) continue;
        unsigned exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }

        /* Compute φ(p^exp). */
        uint64_t ppow = pow_u64(p, exp);          /* p^exp  */
        uint64_t ppow_prev = ppow / p;            /* p^(exp-1)  */
        uint64_t phi = ppow - ppow_prev;          /* φ = p^exp - p^(exp-1) */

        /* Size of image of exponentiation: φ / gcd(e, φ). */
        uint64_t g = gcd64(e, phi);
        uint64_t image = phi / g;
        uint64_t count = image + 1ULL;            /* add the residue 0 */

        result64 *= count;
    }

    /* Any remaining prime factor > sqrt(tmp). */
    if (tmp > 1ULL) {
        uint64_t p = tmp;
        unsigned exp = 1;          /* it appears only once */

        /* φ(p^1) = p-1  */
        uint64_t phi = p - 1ULL;

        uint64_t g = gcd64(e, phi);
        uint64_t image = phi / g;
        uint64_t count = image + 1ULL;            /* zero residue */

        result64 *= count;
    }

    return (seq_t)result64;
}
