/*
 * A366855  Least number such that the concatenation of the exponents in the
 *          prime factorization is n.
 * https://oeis.org/A366855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A366855:  a(n) = least integer whose prime‑factor exponents
    concatenated (decimal) equal n.
*/

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"      /* seq_t big_pow(seq_t base, seq_t exp); */

/* Small prime list – sufficient for any partition of a n up to 5 decimal
   digits (max 5 exponents).  The primes are cast to seq_t to keep the
   type consistent. */
static const seq_t primes[] = { (seq_t)2, (seq_t)3, (seq_t)5, (seq_t)7, (seq_t)11 };

/* Recursive search of all splits of the decimal string s[0..len-1].
   exps[0..cnt-1] holds the exponents of the already chosen parts.
   best contains the current minimal product; found marks whether a
   product has been found yet. */
static void dfs(const char *s, size_t len, size_t pos,
                seq_t *exps, size_t cnt,
                seq_t *best, bool *found)
{
    if (pos == len) {                       /* a full split reached */
        seq_t prod = (seq_t)1;
        for (size_t i = 0; i < cnt; ++i) {
            prod *= big_pow(primes[i], exps[i]);   /* P_i ^ e_i */
        }
        if (!*found || prod < *best) {
            *best  = prod;
            *found = true;
        }
        return;
    }

    /* Leading zeros are not allowed for a part. */
    if (s[pos] == '0')
        return;

    seq_t val = 0;
    for (size_t end = pos + 1; end <= len; ++end) {
        val = val * 10 + (seq_t)(s[end-1] - '0');
        exps[cnt] = val;
        dfs(s, len, end, exps, cnt + 1, best, found);
    }
}

seq_t a(idx_t n)
{
    /* Convert n to its decimal representation in an array of digits. */
    char rev[32];
    size_t rev_len = 0;
    idx_t tmp = n;
    if (tmp == 0) {
        rev[rev_len++] = '0';
    } else {
        while (tmp > 0) {
            rev[rev_len++] = (char)((tmp % 10) + '0');
            tmp /= 10;
        }
    }
    /* Now reverse to obtain the normal order. */
    char s[32];
    for (size_t i = 0; i < rev_len; ++i)
        s[i] = rev[rev_len-1-i];
    size_t len = rev_len;

    /* Prepare recursion buffers. */
    seq_t exps[32];
    seq_t best = 0;
    bool found = false;
    dfs(s, len, 0, exps, 0, &best, &found);

    return best;
}
