/*
 * A357971  a(n) is the number of segments used to represent the time of n
 *          minutes past midnight in the format hh:mm on a 7-segment
 *          calculator display; version where the digits '6', '7' and '9' use
 *          6, 4 and 6 segments, respectively.
 * https://oeis.org/A357971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A010371(A055643(n)) + 6*(4 - ceil(log10(A055643(n) + 1)))
 * Range:     u8 n <= 1439, u16 n <= 1439, u32 n <= 1439, u64 n <= 1439,
 *            u128 n <= 1439, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*         = number of 7‑segment display segments for the time n minutes past midnight (hh:mm) */
/* Period: 1440 minutes (24 hours). */

extern seq_t a(idx_t n);

/* Segment counts for decimal digits 0‑9 on a 7‑segment display.
   Digit 7 uses 4 segments, 6 and 9 use 6 segments. */
static const seq_t segs[10] = {
    6, /* 0 */
    2, /* 1 */
    5, /* 2 */
    5, /* 3 */
    4, /* 4 */
    5, /* 5 */
    6, /* 6 */
    4, /* 7 */
    7, /* 8 */
    6  /* 9 */
};

/* Count decimal digits of a non‑negative integer.
   For 0 it returns 1 (the digit '0').  The result is <= 4. */
static inline int digits_of(idx_t v)
{
    int cnt = 1;
    while (v >= 10) {
        v /= 10;
        ++cnt;
    }
    return cnt;
}

/* Sum of segment counts for the decimal digits of a number.
   The number 0 contributes 0 segments (no digits are displayed). */
static inline seq_t seg_of_number(idx_t v)
{
    if (v == 0)
        return (seq_t)0;
    seq_t sum = 0;
    while (v > 0) {
        uint32_t d = (uint32_t)(v % 10);
        sum += segs[d];
        v /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Reduce to minutes within a day (period 1440). */
    idx_t m = n % 1440ULL;

    /* Compute hours and minutes. */
    idx_t hours   = m / 60;
    idx_t minutes = m % 60;

    /* Form the 4‑digit decimal number hhmm. */
    idx_t hhmm = hours * 100 + minutes;

    /* Segments for the non‑leading digits of hhmm. */
    seq_t seg_no_leading = seg_of_number(hhmm);

    /* Number of leading zeros needed to make a 4‑digit display. */
    int leading_zeros = (hhmm == 0) ? 4 : (4 - digits_of(hhmm));

    /* Each leading zero uses 6 segments. */
    seq_t total = seg_no_leading + ((seq_t)6 * (seq_t)leading_zeros);

    return total;
}
