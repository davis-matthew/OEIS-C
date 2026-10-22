/*
 * A380577  a(n) is the number of distinct compositions of chess pieces with a
 *          collective material value of n that one color in a game can have,
 *          where 0 <= n <= 103.
 * https://oeis.org/A380577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of distinct final piece compositions giving collective material value n
 * under standard chess piece values (P=1,B=3,N=3,R=5,Q=9) and promotion/mortality
 * rules.  Enumerates all tuples (P,B,N,R,Q) of non‑negative integers satisfying
 * P + 3B + 3N + 5R + 9Q = n and
 *   max(0,B-2)+max(0,N-2)+max(0,R-2)+max(0,Q-1) <= 8-P.
 * The sequence is defined for 0 ≤ n ≤ 103; a(n)=0 outside this range. */
#include <stdbool.h>

static seq_t a_cache[104];
static bool a_computed = false;

seq_t a(idx_t n)
{
    /* Return 0 for values outside the defined range */
    if (n > 103) return (seq_t)0;
    if (!a_computed) {
        /* Initialize cache once using brute force enumeration */
        for (idx_t i = 0; i <= 103; ++i) a_cache[i] = (seq_t)0;

        for (idx_t P = 0; P <= 8; ++P) {
            for (idx_t Q = 0; Q <= 11; ++Q) {
                for (idx_t R = 0; R <= 20; ++R) {
                    for (idx_t B = 0; B <= 34; ++B) {
                        for (idx_t N = 0; N <= 34; ++N) {
                            idx_t sum = P + 3 * B + 3 * N + 5 * R + 9 * Q;
                            if (sum > 103) continue;

                            seq_t need =
                                ((B > 2) ? (seq_t)(B - 2) : (seq_t)0) +
                                ((N > 2) ? (seq_t)(N - 2) : (seq_t)0) +
                                ((R > 2) ? (seq_t)(R - 2) : (seq_t)0) +
                                ((Q > 1) ? (seq_t)(Q - 1) : (seq_t)0);

                            if (need <= (seq_t)(8 - P))
                                a_cache[sum] += (seq_t)1;
                        }
                    }
                }
            }
        }
        a_computed = true;
    }
    return a_cache[n];
}
