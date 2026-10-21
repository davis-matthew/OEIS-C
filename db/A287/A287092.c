/*
 * A287092  Strobogrammatic nonpalindromic numbers.
 * https://oeis.org/A287092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 34, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the nth strobogrammatic non‑palindromic number (containing digits 6 or 9). */

#include <stdbool.h>

 /* Mapping of each digit under a 180° rotation. */
static const unsigned char map[10] = {0,1,0,0,0,0,9,0,8,6};

/* Options for the first (most significant) digit – cannot be 0. */
static const unsigned char first_opts[4] = {1,6,8,9};

/* Options for all other outer digits. */
static const unsigned char all_opts[5]  = {0,1,6,8,9};

/* Middle digit options for odd‑length numbers. */
static const unsigned char mid_opts[3]  = {0,1,8};

/* Recursive generator that walks from the outside toward the centre.
   *pos        current outer position
   *end        inner position
   *dig        array holding the partially built digits
   *cnt        counter of valid numbers found so far
   *n          target index
   *res        storage for the nth number
   *stop       flag indicating that the nth number has been found
   *len        total length of the number being generated
*/
static bool gen_rec(int pos, int end, unsigned char *dig,
                    idx_t *cnt, idx_t n, seq_t *res,
                    bool *stop, int len)
{
    if (*stop) return false;                /* already found */

    if (pos > end) {                        /* finished a number   */
        bool has_6or9 = false;
        for (int i = 0; i < len; ++i)
            if (dig[i] == 6 || dig[i] == 9) { has_6or9 = true; break; }

        if (has_6or9) {
            (*cnt)++;
            if (*cnt == n) {                /* nth number reached */
                seq_t v = 0;
                for (int i = 0; i < len; ++i)
                    v = v * 10 + (seq_t)dig[i];
                *res = v;
                *stop = true;
                return true;
            }
        }
        return false;
    }

    if (pos == end) {                       /* middle digit for odd len */
        for (int i = 0; i < 3; ++i) {
            dig[pos] = mid_opts[i];
            if (gen_rec(pos + 1, end, dig, cnt, n, res, stop, len))
                return true;
        }
        return false;
    }

    /* Choose a pair of outer digits. */
    if (pos == 0) {
        for (int i = 0; i < 4; ++i) {       /* first digit cannot be 0 */
            unsigned char x = first_opts[i];
            dig[pos] = x;
            dig[end] = map[x];
            if (gen_rec(pos + 1, end - 1, dig, cnt, n, res, stop, len))
                return true;
        }
    } else {
        for (int i = 0; i < 5; ++i) {
            unsigned char x = all_opts[i];
            dig[pos] = x;
            dig[end] = map[x];
            if (gen_rec(pos + 1, end - 1, dig, cnt, n, res, stop, len))
                return true;
        }
    }
    return false;
}

/* Public interface required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined, guard */
    idx_t cnt = 0;
    seq_t result = 0;
    bool stop = false;
    /* Search incrementally over lengths until the nth term is found. */
    for (int len = 2; ; ++len) {
        unsigned char digits[20];              /* length will never exceed 20 */
        stop = false;
        gen_rec(0, len - 1, digits, &cnt, n, &result, &stop, len);
        if (stop) return result;
    }
}
