/*
 * A333659  a(n) is the greatest number m not yet in the sequence such that
 *          the decimal expansions of n and of m have the same digits (up to
 *          order but with multiplicity).
 * https://oeis.org/A333659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   Computes a(n) of OEIS A333659 by ranking anagrams of n. a(n) is
 *            the largest unused number sharing the same multiset of digits as
 *            n. The algorithm finds the rank of n among all valid
 *            permutations (no leading zeros), then returns the permutation at
 *            the same rank in descending lexicographic order
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t factorial_table[33] = {0};
static bool factorial_inited = false;

// populate factorial table up to 32 (enough for all practical n)
static void init_factorials(void) {
    if (factorial_inited) return;
    factorial_inited = true;
    factorial_table[0] = 1;
    for (int i = 1; i < 33; ++i)
        factorial_table[i] = factorial_table[i-1] * (seq_t)i;
}

static seq_t multinomial(int rem, unsigned char cnt[10]) {
    seq_t num = factorial_table[rem];
    for (int d = 0; d < 10; ++d)
        num /= factorial_table[cnt[d]];
    return num;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;

    init_factorials();

    /* collect digits of n in most‑significant order */
    int rev[32], dcnt = 0;
    idx_t t = n;
    while (t > 0) {
        rev[dcnt++] = t % 10;
        t /= 10;
    }
    int digits[32];
    for (int i = 0; i < dcnt; ++i)
        digits[i] = rev[dcnt-1-i];

    unsigned char cnt[10] = {0};
    for (int i = 0; i < dcnt; ++i) cnt[digits[i]]++;

    int total = dcnt;
    /* rank of n among ascending valid permutations */
    seq_t rank = 0;
    unsigned char tmpc[10];
    for (int i = 0; i < 10; ++i) tmpc[i] = cnt[i];

    for (int pos = 0; pos < total; ++pos) {
        int start = (pos == 0) ? 1 : 0;
        int cur = digits[pos];
        for (int d = start; d < cur; ++d) {
            if (tmpc[d] == 0) continue;
            tmpc[d]--;
            rank += multinomial(total-1-pos, tmpc);
            tmpc[d]++; /* restore */
        }
        tmpc[cur]--;
    }

    /* generate the permutation at this rank in descending lexicographic order */
    unsigned char curc[10];
    for (int i = 0; i < 10; ++i) curc[i] = cnt[i];

    seq_t result = 0;
    seq_t r = rank;

    for (int pos = 0; pos < total; ++pos) {
        int start = (pos == 0) ? 1 : 0;
        for (int d = 9; d >= start; --d) {
            if (curc[d] == 0) continue;
            curc[d]--;
            seq_t perms = multinomial(total-1-pos, curc);
            curc[d]++; /* restore temporarily */

            if (r < perms) {
                result = result * 10 + d;
                curc[d]--; /* permanently use this digit */
                break;
            } else {
                r -= perms;
            }
        }
    }

    return result;
}
