/*
 * A261545  a(n) indicates which letter of the English alphabet is the n-th
 *          most common.
 * https://oeis.org/A261545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = order[(n-1) mod 26], where 'order' is the alphabet index
 *            of the n-th most common letter in English
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t order[26] = {
    /* 1. E (5) */
    /* 2. T (20) */
    /* 3. A (1) */
    /* 4. O (15) */
    /* 5. I (9) */
    /* 6. N (14) */
    /* 7. S (19) */
    /* 8. R (18) */
    /* 9. H (8) */
    /*10. L (12) */
    /*11. D (4) */
    /*12. C (3) */
    /*13. U (21) */
    /*14. M (13) */
    /*15. F (6) */
    /*16. P (16) */
    /*17. G (7) */
    /*18. W (23) */
    /*19. Y (25) */
    /*20. B (2) */
    /*21. V (22) */
    /*22. K (11) */
    /*23. X (24) */
    /*24. J (10) */
    /*25. Q (17) */
    /*26. Z (26) */
    5, 20, 1, 15, 9, 14, 19, 18, 8, 12, 4, 3,
    21, 13, 6, 16, 7, 23, 25, 2, 22, 11, 24, 10,
    17, 26
};

seq_t a(idx_t n) {
    /* Ensure n >= 1; (n-1) is safe for unsigned arithmetic. */
    idx_t idx = (n - 1) % 26;
    return order[idx];
}
