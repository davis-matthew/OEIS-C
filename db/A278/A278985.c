/*
 * A278985  List of words of length n over an alphabet of size 3 that are in
 *          standard order.
 * https://oeis.org/A278985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 63, u32 n <= 9775, u64 n <= 9775,
 *            u128 n <= 9775, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A278985: list of standard‑order words of length n
   over the alphabet {1,2,3}, written as decimal numbers.
   Words are enumerated lexicographically and all words of a
   given length are listed before any longer length.  */

/* The number of words of length L equals
   S(L,1)+S(L,2)+S(L,3) where S are Stirling numbers of the second kind.
   The DP table G[rem][m] counts the number of completions of 'rem'
   positions when the current maximum letter used is 'm' (1≤m≤3).  */

#include <stdbool.h>

/* Maximum word length needed for n up to about 10^4 (the typical limit for
   this project).  */
#define MAXLEN 25

static seq_t G[MAXLEN + 1][4];   /* G[rem][m] for 1 ≤ m ≤ 3 */
static bool G_ready = false;

static void init_G(void)
{
    if (G_ready) return;
    G[0][1] = G[0][2] = G[0][3] = (seq_t)1;
    for (int rem = 1; rem <= MAXLEN; ++rem) {
        G[rem][3] = (seq_t)3 * G[rem - 1][3];
        G[rem][2] = (seq_t)2 * G[rem - 1][2] + G[rem - 1][3];
        G[rem][1] = G[rem - 1][1] + G[rem - 1][2];
    }
    G_ready = true;
}

seq_t a(idx_t n)
{
    /* Index n is 1‑based (a(1) is the first term).  */
    if (n == 0) return (seq_t)0;          /* safety guard */

    init_G();

    seq_t nval = (seq_t)n;
    seq_t cumulative = (seq_t)0;
    int L = 0;     /* word length */

    /* Find the word length L such that the nth word lies in the block
       of length L.  */
    for (int len = 1; len <= MAXLEN; ++len) {
        seq_t cntlen = G[len - 1][1];      /* words of this length */
        if (cumulative + cntlen >= nval) {
            L = len;
            break;
        }
        cumulative += cntlen;
    }
    if (L == 0) L = MAXLEN;  /* defensive, should not happen for n ≤ 10⁴ */

    /* Off‑by‑zero offset within words of length L.  */
    seq_t offset = nval - cumulative - (seq_t)1;

    /* Build the word digit by digit in lexicographic order.  */
    int digits[MAXLEN];
    digits[0] = 1;
    int current_max = 1;

    for (int pos = 1; pos < L; ++pos) {
        int rem = L - pos - 1;                      /* remaining positions */
        int max_digit = (current_max < 3) ? current_max + 1 : current_max;
        for (int v = 1; v <= max_digit; ++v) {
            int next_max = (v > current_max) ? v : current_max;
            seq_t cnt = G[rem][next_max];
            if (offset < cnt) {
                digits[pos] = v;
                current_max = next_max;
                break;
            }
            offset -= cnt;
        }
    }

    /* Convert the digit array to a decimal integer.  */
    seq_t result = (seq_t)0;
    for (int i = 0; i < L; ++i) {
        result = result * (seq_t)10 + (seq_t)digits[i];
    }
    return result;
}
