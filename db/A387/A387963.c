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
 * Validated: full, pass, 104 terms through n = 104 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Test whether a candidate n has the required property. */
static bool is_fixed(uint64_t n)
{
    /* Put decimal digits of n into array s, most significant first. */
    uint8_t s[20];
    int len = 0;
    if (n == 0) {
        s[len++] = 0;
    } else {
        uint64_t t = n;
        while (t > 0) {
            s[len++] = (uint8_t)(t % 10);
            t /= 10;
        }
        /* Reverse to obtain normal order. */
        for (int i = 0; i < len / 2; ++i) {
            uint8_t tmp = s[i];
            s[i] = s[len - 1 - i];
            s[len - 1 - i] = tmp;
        }
    }

    /* Check all substrings of length ≥ 2 that do not start with 0. */
    for (int st = 0; st < len - 1; ++st) {
        if (s[st] == 0) continue;          /* leading zero – skip */
        uint64_t val = 0;
        for (int l = 1; st + l <= len; ++l) {
            val = val * 10 + s[st + l - 1];
            if (l >= 2) {
                if ((val % 2 == 0) || (val % 3 == 0) ||
                    (val % 5 == 0) || (val % 7 == 0))
                    return false;
            }
        }
    }
    return true;
}

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
