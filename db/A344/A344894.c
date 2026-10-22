/*
 * A344894  a(n) is the number of letters in the Bulgarian name of n,
 *          excluding spaces.
 * https://oeis.org/A344894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = number of letters in the Bulgarian name of n (spaces
 *            omitted)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static const unsigned char len_units[10] = {0,4,3,3,6,3,4,5,4,5};
static const unsigned char len_special[10] = {5,10,10,10,13,10,11,12,11,12};
static const unsigned char len_tens[10]   = {0,0,8,8,11,8,9,10,9,10};
static const unsigned char len_hundreds[10] = {0,3,6,6,12,8,10,11,10,11};
static const unsigned char len_thousand[2] = {6,7};          // "хиляда", "хиляди"

/* Recursive helper that returns the length of the Bulgarian name for n */
static seq_t bulgarian_len(idx_t n)
{
    if (n == 0) return 4;                       /* "нула" */
    if (n < 10) return len_units[n];
    if (n < 20) return len_special[n-10];
    if (n < 100) {
        int t = n / 10;
        int u = n % 10;
        if (u == 0) return len_tens[t];
        return len_tens[t] + 1 + len_units[u]; /* 'и' between tens and units */
    }
    if (n < 1000) {
        int h = n / 100;
        int rest = n % 100;
        seq_t len = len_hundreds[h];
        if (rest == 0) return len;
        if (rest % 10 != 0) len += 1;         /* add 'и' */
        return len + bulgarian_len(rest);
    }
    /* n >= 1000 */
    int w = n / 1000;
    int rest = n % 1000;
    seq_t len;
    if (w == 1) {
        len = len_thousand[0];                /* "хиляда" */
    } else {
        len = bulgarian_len(w) + len_thousand[1]; /* thousands part + "хиляди" */
    }
    if (rest == 0) return len;
    if (rest % 10 != 0) len += 1;              /* add 'и' */
    return len + bulgarian_len(rest);
}

seq_t a(idx_t n)
{
    return bulgarian_len(n);
}
