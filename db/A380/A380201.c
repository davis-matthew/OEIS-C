/*
 * A380201  Triangle T(n,k) read by rows, where row n is a permutation of
 *          numbers 1 through n, such that if a deck of n cards is prepared in
 *          this order, and SpellUnder-Down dealing is used, then the
 *          resulting cards are put down in increasing order.
 * https://oeis.org/A380201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, tabl
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A380201 – Triangle T(n,k) (Spell‑Under‑Down inverse Josephus)
    a(n) is the nth element of the triangle read row‑wise (1‑based).
    The function below computes a(n) by constructing the row that
    contains the required element and taking the appropriate entry.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* words lengths for units, teens and tens (0‑9) */
static const int units_len[10] = {0,3,3,5,4,4,3,5,5,4};
static const int teens_len[10] = {3,6,6,8,8,7,7,9,8,8};
static const int tens_len[10]  = {0,0,6,6,5,5,5,7,6,6};
/* magnitude words: thousand, million, billion, trillion */
static const int mag_len[5] = {0,8,7,7,8};

/* length of English words for a number 0‑999 */
static int word_len_group(int g)
{
    int l = 0;
    if (g >= 100) {
        int h = g / 100;
        l += units_len[h];
        l += 7;          /* “hundred” */
        g %= 100;
    }
    if (g >= 20) {
        int t = g / 10;
        l += tens_len[t];
        g %= 10;
    }
    if (g >= 10) {
        l += teens_len[g - 10];
        g = 0;
    }
    if (g > 0) {
        l += units_len[g];
    }
    return l;
}

/* total number of letters used to spell the integer n (n>0)   */
static int letter_count(idx_t n)
{
    if (n == 0) return 0;
    int len = 0;
    int mag = 0;            /* 0 = units, 1 = thousand, … */
    while (n > 0) {
        int g = n % 1000;
        if (g > 0) {
            len += word_len_group(g);
            if (mag > 0) len += mag_len[mag];
        }
        n /= 1000;
        ++mag;
    }
    return len;
}

/* find the row r and column c (1‑based) that contain position n (1‑based) */
static void find_row_col(idx_t n, idx_t *r, idx_t *c)
{
    idx_t rr = 1;
    while (((unsigned __int128)rr * (rr + 1) / 2) < n) ++rr;
    *r = rr;
    /* first position of row r (1‑based) is (r‑1)·r/2 + 1 */
    idx_t first = (unsigned __int128)(rr - 1) * rr / 2;
    *c = n - first;          /* 1‑based column */
}

/*  Main sequence function  */
seq_t a(idx_t n)
{
    idx_t row, col;
    find_row_col(n, &row, &col);

    /* build the Josephus elimination order of positions 0..row‑1 */
    idx_t sz = row;
    idx_t *deck = (idx_t *)malloc(sz * sizeof(idx_t));
    idx_t *order = (idx_t *)malloc(sz * sizeof(idx_t));
    for (idx_t i = 0; i < sz; ++i) deck[i] = i;

    idx_t cur = 0;           /* index of current top card in deck */

    for (idx_t step = 1; step <= row; ++step) {
        int L = letter_count(step);
        idx_t skip = L % sz;
        if (skip > 0) cur = (cur + skip) % sz;

        idx_t pos = deck[cur];       /* position that will be dealt */
        order[step - 1] = pos;

        /* remove deck[cur] from deck */
        for (idx_t j = cur; j < sz - 1; ++j) deck[j] = deck[j + 1];
        --sz;
        if (sz == 0) break;
        if (cur == sz) cur = 0;
    }

    /* the desired row is the inverse of the elimination order */
    seq_t ans = 0;
    for (idx_t i = 0; i < row; ++i) {
        if (order[i] == col - 1) {     /* 0‑based index */
            ans = (seq_t)(i + 1);     /* values are 1‑based */
            break;
        }
    }

    free(order);
    free(deck);
    return ans;
}
