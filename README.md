1. Name: Kammari Harsha vardhan.
   Entry No: 2023MCB1216.
2, Name; Kuthal Ritvik.
   Entry No: 2023MCB1219.
3. Name: Vaibhav Pawar.
   entry No: 2023MCB1317.


Course Instructor: Dr. Anil Shukla.

Mentoring TA: Seema Barda.

Project Title: Segment Tree Implementation for Attack-Based Game.

1.Project Overview:

    This project implements a segment tree data structure specifically designed for managing and querying troop data in an attack-based game. The segment tree efficiently handles various operations related to troop attributes such as attack and health, enabling quick updates and queries that enhance gameplay strategy.

2.Features:

    The segment tree is built to support the following functionalities:

    a.Sum Queries:

        Calculate the total attack or health of troops within a specified range.

    b.Maximum and Minimum Queries:

        Retrieve the index of the troop with the maximum or minimum attack or health in a specified range.

    c.GCD and LCM Queries:

        Compute the greatest common divisor (GCD) and least common multiple (LCM) of troop attack and health values over a specified range.

    d.Dynamic Updates:

        Update troop attributes (attack and health) and ensure all segment trees reflect the changes efficiently.

3.Data Structure

    a.Array of Pairs:

        The segment tree is built on an array of pairs where each pair contains the attack and health values of a troop.

    b.Segment Trees:

        Separate segment trees are maintained for:

        Sum of attacks
        Sum of health
        Maximum attack indices
        Minimum attack indices
        Maximum health indices
        Minimum health indices
        GCD of attacks
        GCD of health
        LCM of attacks
        LCM of health

4.Functions

    a.Building Trees:

        Functions to construct segment trees for sums, max, min, GCD, and LCM recursively.

    b.Querying:

        Functions to query sums, max, min, GCD, and LCM efficiently over specified ranges.

    c.Updating:

        Functions to update troop attributes and reflect changes across all relevant segment trees.

5.Usage

    To use the segment tree:

        a.Initialize the segment tree with an array of troops, each defined by their attack and health values.
        b.Utilize the provided query functions to obtain desired information about the troop array.
        c.Update troop values as needed and perform further queries to analyze the changes.

6.Implementation

    The implementation is done in C++, utilizing recursion for building and querying the segment tree, ensuring that all operations maintain an average time complexity of O(log n).

7.Code Explanation

    Our implementation features a specialized segment tree designed for managing troops in an attack-based game, which operates on an array of pairs representing the attack and health attributes of each troop. The key functionalities include:

    a.Dynamic Attribute Management:

        The segment tree supports dynamic updates to troop attributes, allowing for changes in attack and health values during gameplay without needing to rebuild the entire structure.

    b.Multiple Segment Trees:

        Unlike a typical segment tree that often focuses on a single attribute, your implementation includes multiple segment trees to handle various operations:
            Sum Trees: For calculating total attack and health values over a range.
            Max/Min Index Trees: For quickly retrieving the index of the troop with the highest or lowest attack or health in a specified range.
            GCD and LCM Trees: For computing the GCD and LCM of attack and health values, which can be crucial for strategic gameplay decisions.

    c.Efficient Range Queries:

        The code provides efficient querying capabilities for both sum and index retrieval, maintaining O(log n) time complexity, which is beneficial during fast-paced gameplay where quick decisions are needed.

    d.Update Propagation:

        Updates to troop attributes automatically propagate through the corresponding segment trees, ensuring that all dependent queries reflect the latest data without additional overhead.

8.Unique Implementation Facts

    a.Dual Attribute Handling:

        Your segment tree is tailored to manage two attributes (attack and health) simultaneously, making it distinct from standard implementations that usually handle a single value.

    b.Comprehensive Functionality:

        The combination of sum, min/max, GCD, and LCM capabilities in one data structure is rare. This comprehensive functionality allows players to make informed strategic choices based on various troop characteristics.

    c.Index-Based Queries:

        By maintaining indices for max and min queries, your implementation facilitates efficient troop selection and management, enabling players to respond dynamically to changing battlefield conditions.

    d.Game-Specific Optimization:

        The design considers the specific needs of an attack-based game, allowing for quick decision-making processes that are essential in such environments. The GCD and LCM calculations are particularly useful for analyzing troop synergies or weaknesses.

    e.Segment Tree as a Game Mechanic:

        The segment tree serves not just as a data structure but as an integral part of the game mechanics, influencing how players interact with the game world and strategize their troop deployments.

9.Conclusion

    This segment tree implementation provides a robust solution for efficiently managing and querying troop data in an attack-based game. Its flexibility allows for the integration of various troop attributes and enhances gameplay dynamics through strategic data management. The tailored approach to managing multiple troop attributes efficiently offers players a strategic edge through informed decision-making based on real-time data analysis.

