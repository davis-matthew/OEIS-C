/*
 * A306110  Numbers with digits in {0,...,7} such that every other digit is
 *          strictly less than its neighbors.
 * https://oeis.org/A306110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007094(A032863(n)) Numbers with digits in {0,…,7} such
 *            that every other digit is strictly less than its neighbors. The
 *            function enumerates base‑8 numbers satisfying the property in
 *            increasing order and returns the decimal integer that has the
 *            same digit string. Complexity: O(k·8^k) where...
 * Range:     u8 n <= 56, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static bool good(const int *d, size_t len)
{
    for (size_t i = 1; i < len; i += 2) {
        if (d[i] >= d[i-1]) return false;
        if (i + 1 < len && d[i] >= d[i+1]) return false;
    }
    return true;
}

static bool gen_len(size_t len, size_t pos, int *d,
                    size_t *count, idx_t target,
                    seq_t *res)
{
    if (pos == len) {
        if (!good(d, len)) return false;
        (*count)++;
        if (*count == (size_t)target) {
            seq_t val = 0;
            for (size_t j = 0; j < len; ++j)
                val = val * 10 + d[j];
            *res = val;
            return true;
        }
        return false;
    }

    int start = 0;
    if (len > 1 && pos == 0) start = 1;  /* no leading zero for length>1 */

    for (int dig = start; dig <= 7; ++dig) {
        d[pos] = dig;
        if (gen_len(len, pos + 1, d, count, target, res))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* n is 1‑based */
    size_t need = (size_t)n;
    size_t found = 0;
    seq_t result = 0;

    for (size_t len = 1; ; ++len) {
        if (len == 1) {
            for (int dig = 0; dig <= 7; ++dig) {
                found++;
                if (found == need) return (seq_t)dig;
            }
        } else {
            int digits[20];  /* sufficient for all lengths needed < 10 */
            if (gen_len(len, 0, digits, &found, need, &result))
                return result;
        }
    }
}
