/*
 * A283477  If 2n = 2^e1 + 2^e2 + ... + 2^ek [e1 .. ek distinct], then a(n) =
 *          A002110(e1) * A002110(e2) * ... * A002110(ek).
 * https://oeis.org/A283477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k≥0, bit_k(n)=1} primorial(k+1) where primorial(m) =
 *            ∏_{j=1}^{m} p_j and p_j is the j‑th prime
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 53, u64 n <= 189, u128 n <= 989,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t primorials[65];      /* primorials[1] = 2, primorials[64] = product of first 64 primes */
static bool primorials_inited = false;

/* Build the small table of primorials (first 64 primes). */
static void init_primorials(void)
{
    if (primorials_inited)
        return;

    /* first 64 primes */
    static const unsigned primes[64] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
        31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
        73, 79, 83, 89, 97, 101, 103, 107, 109, 113,
        127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
        179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
        233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
        283, 293, 307, 311
    };

    primorials[0] = (seq_t)1;   /* unused, keeps array 1‑based */
    seq_t prod = (seq_t)1;
    for (idx_t i = 0; i < 64; ++i) {
        prod *= (seq_t)primes[i];
        primorials[i + 1] = prod;   /* primorials[k] = product of first k primes */
    }

    primorials_inited = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_primorials();

    seq_t result = (seq_t)1;
    idx_t m   = n;   /* work on n; bits of 2n are just bits of n shifted left by 1 */
    idx_t pos = 0;   /* bit position in n (0‑based) */

    while (m != 0) {
        if ((m & (idx_t)1) != 0) {
            /* bit at position pos in n is set -> contributes primorial(pos+1) */
            result *= primorials[pos + 1];
        }
        m >>= 1;
        ++pos;
    }

    return result;
}
