-- --------------------------------------------------------

--
-- Table structure for table `worldbroadcast`
--

CREATE TABLE IF NOT EXISTS `worldbroadcast` (
  `entry` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `text` varchar(255) COLLATE utf8_unicode_ci NOT NULL,
  `percent` int(3) NOT NULL DEFAULT '100',
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=3 ;

--
-- Dumping data for table `worldbroadcast`
--

INSERT INTO `worldbroadcast` (`entry`, `text`, `percent`) VALUES
(1, 'You are using Arcemu!', 60),
(2, 'BCSystem Enabled', 100);

