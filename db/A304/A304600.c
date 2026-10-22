/*
 * A304600  Number of nX4 0..1 arrays with every element unequal to 0, 1, 2, 5
 *          or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the number of n×4 binary arrays with upper‑left
 * element 0 satisfying the king‑move restriction.
 * For n > 22 the empirical recurrence from the OEIS entry holds:
 *
 * a(n) =  2*a(n‑1) + 4*a(n‑2) – 5*a(n‑3) + 4*a(n‑4) – 4*a(n‑5)
 *          – 29*a(n‑6) + 18*a(n‑7) + 16*a(n‑8) – 7*a(n‑9)
 *          + 30*a(n‑10) – 8*a(n‑11) – 24*a(n‑12) + 12*a(n‑13)
 *          – 20*a(n‑15) + 12*a(n‑17)
 *
 * Initial values (n = 1 … 22):
 * 8, 9, 23, 73, 179, 515, 1450, 4024, 11122, 31092, 86816,
 * 242134, 674738, 1881798, 5248442, 14637102, 40816980,
 * 113827162, 317437278, 885253890, 2468736666, 6884654742
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for offset 0 */

    /* Memoisation table for previously computed values */
    static seq_t *memo = NULL;
    static idx_t memo_size = 0;

    /* Initial values up to n = 22 */
    static const seq_t init[23] = {
        0ULL,          /* dummy for index 0 */
        8ULL, 9ULL, 23ULL, 73ULL, 179ULL, 515ULL,
        1450ULL, 4024ULL, 11122ULL, 31092ULL, 86816ULL,
        242134ULL, 674738ULL, 1881798ULL, 5248442ULL,
        14637102ULL, 40816980ULL, 113827162ULL, 317437278ULL,
        885253890ULL, 2468736666ULL, 6884654742ULL
    };

    /* Grow memo table to include index n */
    if (memo_size < n) {
        idx_t new_size = n;
        seq_t *p = (seq_t *)realloc(memo, (new_size + 1) * sizeof(seq_t));
        if (!p) return 0;  /* allocation failure – fall back to 0 */
        memo = p;
        for (idx_t i = memo_size + 1; i <= new_size; ++i) {
            if (i <= 22) {
                memo[i] = init[i];
            } else {
                /* Apply empirical recurrence */
                seq_t pos = 0, neg = 0;
                pos += (seq_t)2  * memo[i - 1];
                pos += (seq_t)4  * memo[i - 2];
                pos += (seq_t)4  * memo[i - 4];
                pos += (seq_t)18 * memo[i - 7];
                pos += (seq_t)16 * memo[i - 8];
                pos += (seq_t)30 * memo[i -10];
                pos += (seq_t)12 * memo[i -13];
                pos += (seq_t)12 * memo[i -17];

                neg += (seq_t)5  * memo[i - 3];
                neg += (seq_t)4  * memo[i - 5];
                neg += (seq_t)29 * memo[i - 6];
                neg += (seq_t)7  * memo[i - 9];
                neg += (seq_t)8  * memo[i -11];
                neg += (seq_t)24 * memo[i -12];
                neg += (seq_t)20 * memo[i -15];

                memo[i] = pos - neg;
            }
        }
        memo_size = new_size;
    }

    return memo[n];
}
