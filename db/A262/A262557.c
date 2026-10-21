/*
 * A262557  Numbers with digits in strictly decreasing order, sorted
 *          lexicographically.
 * https://oeis.org/A262557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = lexicographically n-th number whose decimal digits are in
 *            strictly decreasing order (digits 0–9). The sequence has 2^10–1
 *            = 1023 terms; all are generated once and then indexed
 * Range:     u8 n <= 10, u16 n <= 62, u32 n <= 1022, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static seq_t seq_arr[1024];
static bool initialized = false;

static void dfs(idx_t d, seq_t val, idx_t *cnt)
{
    seq_arr[(*cnt)++] = val;
    for (idx_t nxt = 0; nxt < d; ++nxt) {
        seq_t next_val = val * (seq_t)10 + (seq_t)nxt;
        dfs(nxt, next_val, cnt);
    }
}

static void generate(void)
{
    idx_t cnt = 0;
    for (idx_t start = 0; start <= 9; ++start) {
        if (start == 0) {
            seq_arr[cnt++] = (seq_t)0;   /* single digit 0 */
        } else {
            dfs(start, (seq_t)start, &cnt);
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* invalid index, return 0 */
    if (!initialized) { generate(); initialized = true; }
    const idx_t COUNT = 1023;      /* 2^10 – 1 */
    if (n - 1 < COUNT) return seq_arr[n - 1];
    return (seq_t)0;               /* out‑of‑range request */
}
