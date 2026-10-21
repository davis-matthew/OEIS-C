/*
 * A293451  Number of proper divisors of n of the form 4k+1.
 * https://oeis.org/A293451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{d|n, d<n} [d ≡ 1 (mod 4)]
 * Range:     u8 n <= 9818, u16 n <= 9818, u32 n <= 9818, u64 n <= 9818,
 *            u128 n <= 9818, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1U) return 0;

    uint64_t m = (uint64_t)n;
    seq_t even_parity = 1;   /* ways to choose exponents giving overall 1 (mod 4) */
    seq_t odd_parity  = 0;   /* ways giving overall 3 (mod 4) */

    /* Factorise m by trial division */
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p != 0U) continue;

        uint64_t exp = 0;
        while (m % p == 0U) {
            m /= p;
            ++exp;
        }

        if (p == 2U) {
            /* 2^k ≡ 0 or 2 (mod 4) for k≥1; only exponent 0 gives 1 (mod 4).
               Thus ignore 2 completely. */
            continue;
        } else if (p % 4U == 1U) {
            /* Any exponent is acceptable. Multiply both parity counts by (exp+1). */
            seq_t mult = (seq_t)(exp + 1U);
            even_parity = even_parity * mult;
            odd_parity  = odd_parity  * mult;
        } else { /* p % 4 == 3 */
            /* Even exponents → 1 (mod 4); odd exponents → 3 (mod 4).               */
            seq_t even_choices = (seq_t)(exp / 2U + 1U);                     // ⌊exp/2⌋ + 1
            seq_t odd_choices  = (seq_t)(exp / 2U + (exp & 1U));             // ⌈exp/2⌉

            seq_t new_even = even_parity * even_choices + odd_parity * odd_choices;
            seq_t new_odd  = even_parity * odd_choices  + odd_parity * even_choices;
            even_parity = new_even;
            odd_parity  = new_odd;
        }
    }

    /* Remaining prime factor, if any. */
    if (m > 1U) {
        uint64_t p = m;
        if (p == 2U) {
            /* ignore */
        } else if (p % 4U == 1U) {
            seq_t mult = 2U;   /* exp = 1 → exp+1 = 2 */
            even_parity = even_parity * mult;
            odd_parity  = odd_parity  * mult;
        } else { /* p % 4 == 3 */
            seq_t even_choices = 1U;  /* only exponent 0 is even */
            seq_t odd_choices  = 1U;  /* exponent 1 is odd */
            seq_t new_even = even_parity * even_choices + odd_parity * odd_choices;
            seq_t new_odd  = even_parity * odd_choices  + odd_parity * even_choices;
            even_parity = new_even;
            odd_parity  = new_odd;
        }
    }

    /* even_parity now counts all divisors d of n with d ≡ 1 (mod 4),
       including d = n itself when n ≡ 1 (mod 4).  Exclude d = n. */
    if (n % 4U == 1U) {
        even_parity -= 1U;
    }

    return even_parity;
}
