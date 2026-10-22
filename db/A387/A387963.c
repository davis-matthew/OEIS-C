/*
 * A387963  Fixed points of A387819.
 * https://oeis.org/A387963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) are the fixed points of A387819: numbers whose every
 *            decimal substring of length ≥ 2 (whose first digit is not zero)
 *            is not divisible by 2, 3, 5, 7. The sequence is finite; the
 *            largest term is 67379 (104 terms)
 * Range:     u8 n <= 36, u16 n <= 101, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   is_fixed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_fixed.h"

/* Pre‑computed fixed points (maximum 104 elements). */
static seq_t seq_fixed[104];
static bool seq_ready = false;
static idx_t seq_count = 0;

seq_t a(idx_t n)
{
    if (!seq_ready) {
        uint64_t candidate = 1;
        while (seq_count < 104) {             /* up to the known last term */
            if (is_fixed(candidate)) {
                seq_fixed[seq_count++] = (seq_t)candidate;
            }
            ++candidate;
        }
        seq_ready = true;
    }

    if (n > 0 && (idx_t)(n - 1) < seq_count)
        return seq_fixed[n - 1];

    return (seq_t)0;   /* undefined for n > 104 */
}
