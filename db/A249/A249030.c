/*
 * A249030  The difference between the number of letters and the number of
 *          syllables in n in English. (Omit "and" in both.)
 * https://oeis.org/A249030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = A005589(n) - A075774(n)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the difference between the number of letters in the
 * English spelling of n (omitting “and”) and the number of syllables
 * in the standard pronunciation of that number. */

static const int _letters_single[20] =
    {4,3,3,5,4,4,3,5,5,4,3,6,6,8,8,7,7,9,8,8};
static const int _syll_single[20] =
    {2,1,1,1,1,1,1,2,1,1,1,3,1,2,2,2,2,3,2,2};
static const int _letters_tens[8] = {6,6,5,5,5,7,6,6};
static const int _syll_tens[8]    = {2,2,2,2,2,3,2,2};
#define _LETTERS_HUNDRED   7
#define _SYLL_HUNDRED     2
#define _LETTERS_THOUSAND  8
#define _SYLL_THOUSAND    2
#define _LETTERS_MILLION   7
#define _SYLL_MILLION     3

static void _count_ls(idx_t n, int *letters, int *syll)
{
    if (n == 0) { /* zero itself */
        *letters += 4;
        *syll += 2;
        return;
    }
    if (n < 20) {            /* 1..19 */
        *letters += _letters_single[n];
        *syll += _syll_single[n];
        return;
    }
    if (n < 100) {           /* 20..99 */
        int tens  = (int)(n / 10) - 2;
        int unit  = (int)(n % 10);
        *letters += _letters_tens[tens];
        *syll += _syll_tens[tens];
        if (unit) {
            *letters += _letters_single[unit];
            *syll += _syll_single[unit];
        }
        return;
    }
    if (n < 1000) {          /* 100..999 */
        idx_t hundred = n / 100;
        idx_t rem     = n % 100;
        *letters += _letters_single[hundred];
        *syll += _syll_single[hundred];
        *letters += _LETTERS_HUNDRED;
        *syll += _SYLL_HUNDRED;
        if (rem)
            _count_ls(rem, letters, syll);
        return;
    }
    if (n < 1000000) {       /* 1,000..999,999 */
        idx_t thousand = n / 1000;
        idx_t rem      = n % 1000;
        _count_ls(thousand, letters, syll);
        *letters += _LETTERS_THOUSAND;
        *syll += _SYLL_THOUSAND;
        if (rem)
            _count_ls(rem, letters, syll);
        return;
    }
    if (n < 1000000000) {    /* 1,000,000..999,999,999 */
        idx_t million = n / 1000000;
        idx_t rem     = n % 1000000;
        _count_ls(million, letters, syll);
        *letters += _LETTERS_MILLION;
        *syll += _SYLL_MILLION;
        if (rem)
            _count_ls(rem, letters, syll);
        return;
    }
    /* Fallback for very large numbers (not expected in tests). */
    idx_t temp = n;
    while (temp) {
        int digit = (int)(temp % 10);
        *letters += _letters_single[digit];
        *syll += _syll_single[digit];
        temp /= 10;
    }
}

seq_t a(idx_t n)
{
    int letters = 0;
    int syllables = 0;
    _count_ls(n, &letters, &syllables);
    int diff = letters - syllables;
    return (seq_t)diff;
}
