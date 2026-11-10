/*
 * A392743  a(n) = A392736(2*n + 1 - A392736(n)).
 * https://oeis.org/A392743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = M(2n+1-M(n)), where M = A392736 (Meta-Thue-Morse):
 *            M(0)=0, M(1)=1, M(4n)=M(n), M(4n+1)=1-M(n),
 *            M(4n+2)=M(2n+1-M(n)), M(4n+3)=1-M(2n+1-M(n))
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 79 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A392736_a(idx_t n)
{
    if (n < 2)
        return (seq_t)n;

    idx_t q = n >> 2;
    unsigned r = (unsigned)(n & 3U);

    switch (r) {
    case 0:
        return A392736_a(q);
    case 1:
        return 1 - A392736_a(q);
    case 2: {
        seq_t t = A392736_a(q);
        idx_t idx = (q << 1) + 1 - (idx_t)t;
        return A392736_a(idx);
    }
    default: {
        seq_t t = A392736_a(q);
        idx_t idx = (q << 1) + 1 - (idx_t)t;
        return 1 - A392736_a(idx);
    }
    }
}

seq_t a(idx_t n)
{
    seq_t m = A392736_a(n);
    idx_t idx = (n << 1) + 1 - (idx_t)m;
    return A392736_a(idx);
}
