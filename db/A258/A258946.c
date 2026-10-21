/*
 * A258946  Numbers that can be expressed using only the digits 0 and 1 in no
 *          more than three different bases.
 * https://oeis.org/A258946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A258946
   Numbers that can be expressed using only the digits 0 and 1
   in no more than three different bases.
   Formula: The nth term is the nth integer n≥2 that has at most
   three distinct bases k≥2 for which n can be written with digits
   0 and 1 only.  The integer n=1 is omitted.  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static int count_bases(seq_t v)
/* Count distinct bases k≥2 such that the representation of v in base k
   uses only digits 0 and 1.  */
{
    int cnt = 0;
    idx_t vi = (idx_t)v;

    /* base 2 */
    if (vi >= 2) cnt++;

    /* base v-1 */
    if (vi >= 3 && (vi - 1) > 2) cnt++;

    /* base v */
    if (vi > 2) cnt++;

    /* early exit */
    if (cnt > 3) return cnt;

    seq_t sq = floor_sqrt(v);
    idx_t maxb = (idx_t)sq;

    for (idx_t b = 3; b <= maxb && cnt <= 3; ++b) {
        seq_t t = v;
        bool ok = true;
        while (t > 0) {
            seq_t r = t % b;
            if (r > 1) { ok = false; break; }
            t = t / b;
        }
        if (ok) {
            ++cnt;
            if (cnt > 3) break;
        }
    }
    return cnt;
}

seq_t a(idx_t n)
/* Return the nth term of A258946.  Indices start at 1.  */
{
    /* static cache of already computed terms */
    static seq_t *terms = NULL;
    static size_t cap = 0;
    static idx_t count = 0;   /* number of terms stored */
    static idx_t nextnum = 2; /* next integer to test (start from 2) */

    if (n == 0) return 0;   /* behaviour for indices below offset */

    if (count < n) {
        if (!terms) {
            cap = 64;
            terms = (seq_t *)malloc(cap * sizeof(seq_t));
        }
        while (count < n) {
            seq_t v = (seq_t)nextnum;
            if (count_bases(v) <= 3) {
                if (count >= (idx_t)cap) {
                    cap *= 2;
                    terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
                }
                terms[count++] = v;
            }
            ++nextnum;
        }
    }
    return terms[n - 1];
}
