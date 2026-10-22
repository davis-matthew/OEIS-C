/*
 * A387685  a(n) is the number of geometric progressions of three or more
 *          positive integers, strictly increasing with sum n.
 * https://oeis.org/A387685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of tuples (a,k,p,q) with a>0,k≥3,p>q≥1, gcd(p,q)=1
 *            such that a = c·q^(k‑1), r = p/q, and c·(p^k – q^k)/(p – q) = n
 * Range:     u8 n <= 1445, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_seqt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A387685 – number of strictly increasing integer geometric progressions,
    length ≥ 3, whose terms sum to n.  */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd_seqt.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t nSeq  = (seq_t)n;
    unsigned long long limit = (unsigned long long)n;

    /* maximum k such that 2^k – 1 ≤ n */
    idx_t maxK = 0;
    unsigned long long pow2 = 2;
    while (pow2 - 1 <= limit) {
        ++maxK;
        pow2 <<= 1;
    }
    if (maxK < 3) return 0;

    for (idx_t k = 3; k <= maxK; ++k) {
        for (idx_t q = 1; /* infinite */; ++q) {
            /* compute q^(k‑1) with early stop */
            unsigned long long q_pow = 1;
            for (idx_t i = 0; i < k - 1; ++i) {
                if (q_pow > limit / q) { q_pow = limit + 1; break; }
                q_pow *= q;
            }
            if (q_pow > limit) break;                /* further q give larger sums */

            for (idx_t p = q + 1; ; ++p) {
                if (gcd_seqt(p, q) != 1) continue;    /* ratio must be reduced */

                /* compute the base sum S = (p^k – q^k)/(p – q) via iterative terms */
                unsigned long long sum = 0;
                unsigned long long term = q_pow;        /* q^(k‑1) */
                for (idx_t i = 0; i < k; ++i) {
                    sum += term;
                    if (sum > limit) break;
                    /* next term = term * p / q ; safe because term ≤ limit */
                    term = (term * p) / q;
                }

                if (sum > limit) break;                 /* larger p only increases sum */
                if (nSeq % sum == 0) ++count;
            }
        }
    }
    return count;
}
