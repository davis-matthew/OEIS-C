/*
 * A367807  Even palindromes, divided by 2.
 * https://oeis.org/A367807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = A029951(n)/2, where A029951 is the increasing sequence of
 *            even palindromes (numbers that are palindromes and even)
 * Range:     u8 n <= 15, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* pow10[i] = 10^i  (i <= 19 is enough for n ≤ 10000) */
static const idx_t pow10[20] = {
    1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL,
    100000ULL, 1000000ULL, 10000000ULL, 100000000ULL,
    1000000000ULL, 10000000000ULL, 100000000000ULL,
    1000000000000ULL, 10000000000000ULL, 100000000000000ULL,
    1000000000000000ULL, 10000000000000000ULL,
    100000000000000000ULL, 1000000000000000000ULL,
    10000000000000000000ULL
};

/* Build an even palindrome of length L given a firstHalf.
   firstHalf contains the half digits (including the middle digit for odd L).
   Returns the full palindrome. */
static seq_t make_palindrome(idx_t L, seq_t firstHalf) {
    seq_t pal = firstHalf;
    seq_t tmp = firstHalf;
    if (L & 1U) {              /* odd length: drop middle digit */
        tmp /= 10;
    }
    while (tmp > 0) {
        pal = pal * 10 + (tmp % 10);
        tmp /= 10;
    }
    return pal;
}

/* Main sequence function */
seq_t a(idx_t n) {
    idx_t count = 0;          /* number of palindromes processed */
    if (n == 0) {             /* undefined but guard */
        return 0;
    }

    /* Length 1: 0,2,4,6,8 */
    static const seq_t single[5] = {0ULL, 2ULL, 4ULL, 6ULL, 8ULL};
    for (idx_t i = 0; i < 5; ++i) {
        ++count;
        if (count == n) {
            return single[i] / 2;
        }
    }

    /* For lengths 2 upwards */
    for (idx_t L = 2; ; ++L) {
        idx_t k = (L + 1) / 2;                     /* half length (ceil) */
        idx_t leading = k - 1;                    /* digits after first */

        /* Iterate over first digit: 2,4,6,8 */
        for (seq_t firstDigit = 2; firstDigit <= 8; firstDigit += 2) {
            seq_t base = firstDigit * (seq_t)pow10[leading];
            /* Iterate over all combinations of the remaining digits */
            for (idx_t rest = 0; rest < pow10[leading]; ++rest) {
                seq_t firstHalf = base + (seq_t)rest;
                seq_t pal = make_palindrome(L, firstHalf);
                ++count;
                if (count == n) {
                    return pal / 2;
                }
            }
        }
    }
}
