/*
 * A368356  Numbers k such that A121805(k) is prime.
 * https://oeis.org/A368356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = k-th index m (increasing) such that A121805(m) is prime;
 *            A121805: c(1)=1, c(m)=c(m-1)+10*(c(m-1) mod 10)+y, y the least
 *            digit equal to the leading digit of c(m); computed by direct
 *            scan
 * Range:     u8 n <= 1, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool A368356_isprime(seq_t x)
{
    if (x < 2) return false;
    if (x < 4) return true;
    if (x % 2 == 0) return false;
    for (seq_t i = 3; i * i <= x; i += 2)
        if (x % i == 0) return false;
    return true;
}

static seq_t A368356_msd(seq_t x)
{
    while (x >= 10) x /= 10;
    return x;
}

static seq_t *A368356_found = NULL;   /* 1-based list of indices */
static size_t A368356_cnt = 0, A368356_cap = 0;
static seq_t A368356_cur = 0;
static seq_t A368356_idx = 0;
static bool A368356_done = false;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (A368356_cnt < (size_t)n && !A368356_done) {
        if (A368356_idx == 0) {
            A368356_cur = 1;
            A368356_idx = 1;
        } else {
            seq_t prev = A368356_cur;
            seq_t base = prev + (prev % 10) * 10;
            seq_t val = 0;
            for (seq_t d = 0; d <= 9; ++d) {
                seq_t cand = base + d;
                if (A368356_msd(cand) == d) { val = cand; break; }
            }
            if (val == 0) { A368356_done = true; break; }
            A368356_cur = val;
            A368356_idx++;
        }
        if (A368356_isprime(A368356_cur)) {
            if (A368356_cnt + 1 >= A368356_cap) {
                A368356_cap = A368356_cap ? A368356_cap * 2 : 256;
                A368356_found = mem_resize(seq_t, A368356_found, A368356_cap + 1);
            }
            A368356_found[++A368356_cnt] = A368356_idx;
        }
    }
    if ((size_t)n <= A368356_cnt) return A368356_found[(size_t)n];
    return (seq_t)0;
}
