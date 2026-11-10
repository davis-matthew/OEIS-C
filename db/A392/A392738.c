/*
 * A392738  Positions of 0's in A392736.
 * https://oeis.org/A392738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   A392736 has exactly two 0's in each block 4q..4q+3, so with
 *            q=(n-1)\2, r=(n-1) mod 2: a(n) = 4q + (A392736(q)==0 ? 0 : 1) if
 *            r=0; a(n) = 4q + (A392736(4q+2)==0 ? 2 : 3) if r=1
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A392736_a(idx_t n)
{
    if (n < 2)
        return (int)n;
    idx_t q = n >> 2;
    unsigned r = (unsigned)(n & 3U);
    switch (r) {
    case 0:
        return A392736_a(q);
    case 1:
        return 1 - A392736_a(q);
    case 2: {
        int t = A392736_a(q);
        return A392736_a((q << 1) + 1 - (idx_t)t);
    }
    default: {
        int t = A392736_a(q);
        return 1 - A392736_a((q << 1) + 1 - (idx_t)t);
    }
    }
}

seq_t a(idx_t n)
{
    idx_t m = n - 1;
    idx_t q = m >> 1;
    idx_t base = q << 2;
    seq_t res = (seq_t)q * 4;
    if ((m & 1U) == 0) {
        if (A392736_a(q) != 0)
            res += 1;
    } else {
        if (A392736_a(base + 2) == 0)
            res += 2;
        else
            res += 3;
    }
    return res;
}
