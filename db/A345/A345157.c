/*
 * A345157  a(1) = 1; thereafter, a(n) = A073327(a(n-1)), using the US English
 *          version of A073327 when there is a choice.
 * https://oeis.org/A345157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(1) = 1; for n > 1, a(n) = A073327(a(n−1)), where A073327(n) is
 *            the sum of letter values of the US English name of n (A=1, B=2,
 *            …, Z=26)
 * Range:     u8 n <= 5, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Letter‑value sums for the words 0 … 19 (clear spelling values). */
static const uint16_t small_sum[20] = {
     64, /* ZERO  */
     34, /* ONE   */
     58, /* TWO   */
     56, /* THREE */
     60, /* FOUR  */
     42, /* FIVE  */
     52, /* SIX   */
     65, /* SEVEN */
     49, /* EIGHT */
     42, /* NINE  */
     39, /* TEN   */
     63, /* ELEVEN*/
     87, /* TWELVE*/
     99, /* THIRTEEN*/
    104, /* FOURTEEN*/
     65, /* FIFTEEN*/
     96, /* SIXTEEN */
    109, /* SEVENTEEN*/
     73, /* EIGHTEEN*/
     86  /* NINETEEN*/
};

/* Letter‑value sums for the tens 20 … 90 (index 0‑9). */
static const uint16_t tens_sum[10] = {
     0,   /* 0  */
     0,   /* 10 */
   107,  /* TWENTY */
   100,  /* THIRTY */
    84,  /* FORTY */
    66,  /* FIFTY */
    97,  /* SIXTY */
   110,  /* SEVENTY */
    74,  /* EIGHTY */
    87   /* NINETY */
};

static const uint16_t HUNDRED_SUM = 74; /* HUNDRED */

/* Compute the letter‑value sum of the US English name of a number
   less than 1000.  The function accepts a seq_t value but only
   uses the low 64 bits of it; it will never be called with numbers
   > 999 for the intended sequence. */
static seq_t word_letter_sum(seq_t v)
{
    /* Convert to plain flat value; the caller guarantees v < 1000. */
    uint64_t x = (uint64_t)v;
    if (x == 0)
        return (seq_t)small_sum[0];

    uint32_t sum = 0;
    if (x >= 100) {
        uint32_t hundreds = x / 100;            /* 1‑9 */
        sum += small_sum[hundreds];
        sum += HUNDRED_SUM;
        x %= 100;
    }
    if (x > 0) {
        if (x < 20) {
            sum += small_sum[(uint16_t)x];
        } else {
            uint32_t tens = x / 10;             /* 2‑9 */
            sum += tens_sum[tens];
            uint32_t unit = x % 10;             /* 0‑9 */
            if (unit > 0)
                sum += small_sum[unit];
        }
    }
    return (seq_t)sum;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t value = (seq_t)1;
    for (idx_t i = 1; i < n; ++i) {
        value = word_letter_sum(value);
    }
    return value;
}
