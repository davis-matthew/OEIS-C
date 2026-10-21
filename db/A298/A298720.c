/*
 * A298720  EBCDIC codes for lower case letters.
 * https://oeis.org/A298720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: EBCDIC codes for lower case letters.
   Mapping:
   a(1)=129,  a(2)=130, … , a(9)=137,
   a(10)=145…a(18)=153,
   a(19)=162…a(26)=169.
   (Values above 26 are undefined.) */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for 0, but return 0 to keep behaviour sane. */
        return (seq_t)0;
    }

    if (n <= 9) {
        return (seq_t)128 + (seq_t)n;          /* 0x81‑0x89 */
    } else if (n <= 18) {
        return (seq_t)144 + ((seq_t)n - 9);    /* 0x91‑0x99 */
    } else if (n <= 26) {
        return (seq_t)162 + ((seq_t)n - 19);   /* 0xA2‑0xA9 */
    } else {
        /* Undefined beyond 26; return 0. */
        return (seq_t)0;
    }
}
