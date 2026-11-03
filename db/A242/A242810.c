/*
 * A242810  a(n) is the smallest n-digit number whose truncation after its
 *          first k digits is divisible by the k-th Lucas number (A000032(n))
 *          for k = 1..n.
 * https://oeis.org/A242810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: full, pass, 9 terms through n = 9 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): minimal n‑digit number whose first k digits are divisible by Lucas(k) (1≤k≤n). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

static bool build_recursive(idx_t pos, seq_t prefix, idx_t n,
                            seq_t *result, seq_t *lu)
{
    if (pos == n) {                /* n digits built */
        *result = prefix;
        return true;
    }

    idx_t k = pos + 1;             /* position of next digit (1‑based) */

    if (k >= 5) {                  /* deterministic digit */
        seq_t mod   = lu[k];
        seq_t rem   = prefix % mod;
        seq_t val   = (rem * (seq_t)10) % mod;
        seq_t digit = (mod - val) % mod;   /* the unique solution modulo mod */

        if (digit >= 10) return false;    /* no 0‑9 digit satisfies it */
        seq_t new_prefix = prefix * (seq_t)10 + digit;
        return build_recursive(pos + 1, new_prefix, n, result, lu);
    } else {                       /* search 0‑9 digits (or 1‑9 for first) */
        int start = (pos == 0) ? 1 : 0;
        for (int d = start; d <= 9; ++d) {
            seq_t new_prefix = prefix * (seq_t)10 + (seq_t)d;
            seq_t mod = lu[k];
            if (new_prefix % mod == 0) {
                if (build_recursive(pos + 1, new_prefix, n, result, lu))
                    return true;
            }
        }
        return false;
    }
}

static seq_t *lucas = NULL;
static idx_t lucas_len = 0;

static void ensure_lucas(idx_t n)
{
    if (lucas_len <= n) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = mem_resize(seq_t, lucas, new_len);
        if (!tmp) exit(1);            /* out of memory */
        lucas = tmp;

        if (lucas_len == 0) {
            lucas[0] = (seq_t)2;
            lucas[1] = (seq_t)1;
        }
        /* compute missing terms */
        for (idx_t i = lucas_len; i <= n; ++i) {
            if (i == 0) continue;
            if (i == 1) continue;
            lucas[i] = lucas[i-1] + lucas[i-2];
        }
        lucas_len = new_len;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; behaviour undefined for n<1 */
    ensure_lucas(n);
    seq_t res = 0;
    build_recursive(0, 0, n, &res, lucas);
    return res;
}
