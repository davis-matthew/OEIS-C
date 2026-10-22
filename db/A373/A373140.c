/*
 * A373140  Numbers k such that A276085(k) is a multiple of 5, where A276085
 *          is the primorial base log-function.
 * https://oeis.org/A373140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that A276085(k) (primorial‑base log) is a multiple of 5.
   Membership test: if e₂,e₃,e₅ are the exponents of 2, 3, 5 in k,
   then k ∈ S ⇔ (e₂ − 3·e₃ − 4·e₅) ≡ 0 (mod 5).   All other prime factors
   contribute 0 modulo 5. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */
    idx_t count = 0;
    for (seq_t k = 1; ; ++k) {
        seq_t tmp = k;
        idx_t e2 = 0, e3 = 0, e5 = 0;

        /* factor powers of 2, 3, 5 */
        while ((tmp & 1) == 0) { tmp >>= 1; ++e2; }
        while (tmp % 3 == 0) { tmp /= 3; ++e3; }
        while (tmp % 5 == 0) { tmp /= 5; ++e5; }

        /* compute the linear combination as a signed value */
        int64_t diff = (int64_t)e2 - 3LL * (int64_t)e3 - 4LL * (int64_t)e5;
        int mod = (int)((diff % 5 + 5) % 5);  /* canonical 0..4 */

        if (mod == 0) {
            ++count;
            if (count == n) return k;
        }
    }
}
