/*
 * A271639  Orphans: integers without ancestors, in the sense explained below.
 * https://oeis.org/A271639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer that has no parent under the
 *            successive-absolute-differences mapping (Orphans A271639)
 * Range:     u8 none, u16 n <= 7394, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_to_str.h"

/* Return true if the number x has no parent. */
static bool is_orphan(const seq_t x)
{
    /* Convert x to a decimal string to examine its digits. */
    char buf[128];
    size_t len = 0;
    seq_to_str(x, buf, &len);
    if (len == 0)
        return true;  /* should not happen for positive integers */

    /* DP over possible parent digits.
       possible[d] == true means there is a valid parent prefix ending with digit d. */
    bool possible[10] = {0};
    bool next[10]   = {0};

    /* Parent's first digit cannot be 0. */
    for (int d = 1; d <= 9; ++d)
        possible[d] = true;

    /* For each child digit, propagate possible parent digits. */
    for (size_t i = 0; i < len; ++i) {
        unsigned digit = buf[i] - '0';  /* current child digit (difference) */

        for (int d = 0; d <= 9; ++d)
            next[d] = false;

        for (int cur = 0; cur <= 9; ++cur) if (possible[cur]) {
            int nxt = cur + digit;
            if (nxt >= 0 && nxt <= 9) next[nxt] = true;
            nxt = cur - digit;
            if (nxt >= 0 && nxt <= 9) next[nxt] = true;
        }
        /* Move to next state */
        for (int d = 0; d <= 9; ++d)
            possible[d] = next[d];
    }

    /* If any digit is possible for the final parent position, a parent exists. */
    for (int d = 0; d <= 9; ++d)
        if (possible[d])
            return false;  /* has ancestor */

    return true;  /* orphan */
}

seq_t a(idx_t n)
{
    idx_t orphan_cnt = 0;
    idx_t candidate  = 1;

    while (true) {
        seq_t val = (seq_t)candidate;
        if (is_orphan(val)) {
            orphan_cnt++;
            if (orphan_cnt == n)
                return val;
        }
        candidate++;
    }
}
