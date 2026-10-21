/*
 * A265527  Largest base-2 palindrome m <= 2n, written in base 10.
 * https://oeis.org/A265527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the largest binary palindrome not exceeding 2*n. */

/* Count the number of bits needed to represent x (x > 0). */
static inline unsigned bitlen_seq_t(seq_t x)
{
    unsigned len = 0;
    while (x != 0) {
        x >>= 1;
        ++len;
    }
    return len;
}

/* Build a palindrome of total_len bits from a left prefix.
   The prefix must contain (total_len+1)/2 bits. */
static inline seq_t make_pal(seq_t left, unsigned total_len)
{
    unsigned half = total_len >> 1;   /* number of bits to mirror */
    seq_t rev = (seq_t)0;
    seq_t t   = left;

    /* If the length is odd, skip the middle bit before mirroring. */
    if (total_len & 1)
        t >>= 1;

    for (unsigned i = 0; i < half; ++i) {
        rev = (rev << 1) | (t & (seq_t)1);
        t >>= 1;
    }

    return (left << half) | rev;
}

seq_t a(idx_t n)
{
    /* Compute 2*n in seq_t to avoid overflow for small seq_t types. */
    seq_t limit = (seq_t)2 * (seq_t)n;
    if (limit == 0)
        return 0;          /* a(0) = 0 */

    unsigned len = bitlen_seq_t(limit);          /* len >= 1 */
    unsigned leftlen = (len + 1) >> 1;           /* (len+1)/2 */
    seq_t left = limit >> (len - leftlen);       /* high leftlen bits of limit */

    seq_t cand = make_pal(left, len);
    if (cand <= limit)
        return cand;

    seq_t best = 0;

    /* Candidate with left prefix decremented by 1 (same length). */
    if (left > 0) {
        seq_t cand1 = make_pal(left - (seq_t)1, len);
        if (cand1 <= limit && cand1 > best)
            best = cand1;
    }

    /* Candidate of length len-1 consisting of all ones (largest palindrome
       with shorter length). This is (1<<(len-1))-1. */
    if (len > 1) {
        seq_t cand2 = ((seq_t)1 << (len - 1)) - (seq_t)1;
        if (cand2 <= limit && cand2 > best)
            best = cand2;
    }

    return best;
}
