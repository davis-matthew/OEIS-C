/*
 * A295742  Numbers that have exactly two representations of a sum of seven
 *          nonnegative squares.
 * https://oeis.org/A295742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Enumerate all integers up to 5408 having exactly two unordered
   representations as a sum of seven nonnegative squares and cache the list.
*/

#include <stdlib.h>
#include <string.h>

static seq_t *list   = NULL;   /* numbers with exactly two representations   */
static size_t list_len = 0;    /* number of elements in list                 */
static size_t list_cap = 0;    /* allocated capacity of list                 */

#define MAX_N 5408

static void init_sequence(void)
{
    if (list != NULL)
        return; /* already built */

    /* dp[used][sum] counts unordered representations using `used` squares
       that sum to `sum`.  Values are saturated at 3 (≥3). */
    static uint8_t dp[8][MAX_N + 1];
    static uint8_t ndp[8][MAX_N + 1];

    /* initialise */
    memset(dp, 0, sizeof(dp));
    dp[0][0] = 1;

    /* iterate over each possible square value */
    for (int s = 0; (idx_t)s * (idx_t)s <= MAX_N; ++s) {
        int val = s * s;
        memset(ndp, 0, sizeof(ndp));

        for (int used = 0; used <= 7; ++used) {
            for (int sum = 0; sum <= MAX_N; ++sum) {
                uint8_t cur = dp[used][sum];
                if (cur == 0)
                    continue;

                for (int k = 0; k <= 7 - used; ++k) {
                    int newsum = sum + k * val;
                    if (newsum > MAX_N)
                        break;
                    uint8_t ref = ndp[used + k][newsum];
                    ref += cur;
                    if (ref > 2)
                        ref = 3;                     /* mark >=3   */
                    ndp[used + k][newsum] = ref;
                }
            }
        }
        memcpy(dp, ndp, sizeof(dp));
    }

    /* collect numbers with exactly two representations */
    list_cap = 64;
    list = (seq_t *)malloc(list_cap * sizeof(seq_t));
    list_len = 0;

    for (idx_t n = 1; n <= MAX_N; ++n) {
        if (dp[7][n] == 2) {
            if (list_len == list_cap) {
                list_cap <<= 1;
                list = (seq_t *)realloc(list, list_cap * sizeof(seq_t));
            }
            list[list_len++] = (seq_t)n;
        }
    }
}

seq_t a(idx_t n)
{
    init_sequence();
    if (n == 0 || n - 1 >= list_len)
        return 0;   /* undefined for out‑of‑range indices */
    return list[n - 1];
}
