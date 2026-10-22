/*
 * A384128  Number of iterations for the circular absolute first-difference on
 *          decimal digits to reach a repdigit.
 * https://oeis.org/A384128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the number of iterations of the circular absolute
   first‑difference on the decimal digits of n until a repdigit is
   reached.  The implementation simulates the process directly. */
#include <stdbool.h>
#include <stddef.h>

/* Return the number of steps required for n to become a repdigit under
   the circular absolute first‑difference map.  For n with all equal
   digits the result is 0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defined only for n ≥ 1, the guard is defensive */

    seq_t cur  = (seq_t)n;
    seq_t step = 0;

    /* temporary buffers – length never exceeds 128 decimal digits for
       the tested range of n (and sequence values stay well within 64 bits). */
    uint8_t dig[128];
    size_t  len;

    for (;;) {
        /* ---- extract digits most‑significant first ---- */
        if (cur == 0) {
            len = 1;
            dig[0] = 0;
        } else {
            len = 0;
            seq_t tmp = cur;
            /* dig holds least‑significant first, then reverse */
            while (tmp > 0) {
                dig[len++] = (uint8_t)(tmp % 10);
                tmp /= 10;
            }
            /* reverse to most‑significant order */
            for (size_t i = 0; i < len / 2; ++i) {
                uint8_t t = dig[i];
                dig[i] = dig[len - 1 - i];
                dig[len - 1 - i] = t;
            }
        }

        /* ---- test for repdigit ---- */
        bool rep = true;
        for (size_t i = 1; i < len; ++i) {
            if (dig[i] != dig[0]) {
                rep = false;
                break;
            }
        }
        if (rep) return step;

        /* ---- compute next number ---- */
        seq_t next = 0;
        for (size_t i = 0; i < len; ++i) {
            unsigned int a = dig[i];
            unsigned int b = dig[(i + 1) % len];
            unsigned int diff = (a > b) ? a - b : b - a;
            next = next * 10 + diff;
        }

        cur  = next;
        step += 1;
    }
}
